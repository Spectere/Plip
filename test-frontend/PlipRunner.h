// PlipRunner.h
//
// An automated Plip core runner.

#pragma once

#include <atomic>
#include <chrono>
#include <format>
#include <set>
#include <thread>
#include <vector>

#include "MockPlip/MockAudio.h"
#include "MockPlip/MockVideo.h"
#include "Models/FrontendConfig.h"
#include "Models/TestResults.h"
#include "Models/UnitTest.h"
#include "PlipInstance.h"
#include "ResultComparator.h"

namespace fs = std::filesystem;

class PlipRunner {
public:
    [[nodiscard]] static std::set<TestResult> RunTests(const FrontendConfig& testConfig, const std::set<UnitTest>& unitTests) {
        if(testConfig.Threads == 1) {
            // Single-threaded, serialized method.
            std::set<TestResult> testResults;
            for(const auto& unitTest : unitTests) {
                for(auto i = 0; i < unitTest.Subtests.size(); ++i) {
                    testResults.insert(RunTest(testConfig, unitTest, i));
                }
            }

            return testResults;
        }

        //
        // threads go brrrrr
        //

        // Flatten tests prior to dispatch.
        std::vector<std::pair<const UnitTest*, size_t>> tasks;
        for(const auto& unitTest : unitTests) {
            for(size_t i = 0; i < unitTest.Subtests.size(); ++i) {
                tasks.emplace_back(&unitTest, i);
            }
        }

        std::vector<TestResult> results(tasks.size());
        const size_t workerCount = std::max<uint64_t>(1, std::min<uint64_t>(testConfig.Threads, tasks.size()));
        std::atomic<size_t> next {};

        auto worker = [&] {
            for(size_t i = next.fetch_add(1); i < tasks.size(); i = next.fetch_add(1)) {
                const auto &[ unitTest, subtestId ] = tasks[i];
                try {
                    results[i] = RunTest(testConfig, *unitTest, subtestId);
                } catch(const std::exception& ex) {
                    TestResult result {};
                    result.Key = FormatKey(*unitTest, subtestId);
                    result.SubtestId = subtestId;
                    result.Test = *unitTest;
                    result.RunnerError = std::format("Unhandled exception: {}", ex.what());
                    results[i] = std::move(result);
                }
            }
        };

        std::vector<std::thread> threadPool;
        threadPool.reserve(workerCount);

        for(size_t i = 0; i < workerCount; ++i) {
            threadPool.emplace_back(worker);
        }

        for(auto& thread : threadPool) {
            thread.join();
        }

        return std::set(
            std::make_move_iterator(results.begin()),
            std::make_move_iterator(results.end())
        );
    }

private:
    static std::string FormatKey(const UnitTest& unitTest, const size_t subtestId) {
        return unitTest.Key + "-" + unitTest.Subtests[subtestId].Name;
    }

    static Plip::PlipKeyValuePairCollection GetPlipKvp(const FrontendConfig& testConfig, const Subtest& subtest) {
        std::unordered_map<std::string, std::string> coreConfig {};
        for(auto &[ key, value ] : subtest.Config) {
            const auto assetTag = value.find("{assets}");

            // Keys in plip.conf are read in as lowercase. Cores expect this behavior.
            auto newKey = key;
            std::ranges::transform(newKey, newKey.begin(), tolower);

            auto newValue = value;
            if(assetTag != std::string::npos) {
                newValue.replace(assetTag, strlen("{assets}"), testConfig.AssetsDirectory);
            }

            coreConfig.insert({ newKey, newValue });
        }

        return Plip::PlipKeyValuePairCollection(coreConfig);
    }

    static std::tuple<bool, Plip::PlipValidCore> MatchCore(const std::string& coreName) {
        const auto coreList = Plip::PlipInstance::GetSupportedCores();
        bool found {};
        Plip::PlipValidCore coreTag {};
        for(const auto core : coreList) {
            if(core.name == coreName) {
                found = true;
                coreTag = core.descriptor;
                break;
            }
        }

        return { found, coreTag };
    }

    static TestResult RunTest(const FrontendConfig& testConfig, const UnitTest& unitTest, const size_t subtestId) {
        using namespace std::chrono;

        TestResult result {};

        result.Key = FormatKey(unitTest, subtestId);
        result.SubtestId = subtestId;
        result.Test = unitTest;

        const auto subtest = unitTest.Subtests[subtestId];

        // Do some quick sanity checks.
        if(unitTest.Stop.Type == StopType::None) {
            result.InitializationError = std::format("Invalid stop condition for test: {}", unitTest.Filename);
            return result;
        }

        if(subtest.Expected.Type == ResultType::None) {
            result.InitializationError = std::format("Invalid expected value type for subtest '{}' in test: {}", subtest.Name, unitTest.Filename);
            return result;
        }

        // Create the Plip instance.
        const auto [ coreFound, coreTag ] = MatchCore(unitTest.Core);
        if(!coreFound) {
            result.InitializationError = std::format("Core '{}' could not be found for test: {}", unitTest.Core, unitTest.Filename);
            return result;
        }

        const auto coreConfigKvp = GetPlipKvp(testConfig, subtest);

        const auto mockAudio = std::make_unique<MockAudio>();
        const auto mockVideo = std::make_unique<MockVideo>();
        const auto plip = std::make_unique<Plip::PlipInstance>(mockVideo.get(), mockAudio.get());

        const auto testFileDirectory = fs::canonical(unitTest.Filename).parent_path();
        const auto romPath = testFileDirectory / unitTest.Rom;
        plip->Load(coreTag, romPath, coreConfigKvp);
        plip->GetCore()->SetHeadless(true);

        uint64_t cycle {};
        const auto start = steady_clock::now();
        bool running = true;
        while(running) {
            ++cycle;
            plip->Step();

            // Handle exit condition.
            switch(unitTest.Stop.Type) {
                case StopType::Cycles:
                    if(cycle >= unitTest.Stop.Value.ValueUInt) {
                        running = false;
                    }
                    break;
                case StopType::Opcode:
                    if(plip->GetCore()->GetLastExecutedOpcode() == unitTest.Stop.Value.ValueUInt) {
                        running = false;
                    }
                    break;
                case StopType::None:
                default:
                    throw std::runtime_error("BUG: Invalid stop event was not caught early!");
            }

            // Quick little safeguard...
            if(cycle >= testConfig.BailoutCycles) {
                result.TimedOut = true;
                break;
            }
        }

        const auto end = steady_clock::now();
        const auto time = end - start;
        result.Cycles = cycle;
        result.Duration = static_cast<double>(duration_cast<milliseconds>(time).count()) / 1000.0f;

        // Record results.
        result.Actual.Type = subtest.Expected.Type;
        switch(subtest.Expected.Type) {
            case ResultType::Image: {
                result.Actual.Value.ValueString = result.Key + "-actual.png";
                fs::path resultImagePath = testConfig.ResultsDirectory / "images" / result.Actual.Value.ValueString;
                mockVideo->SaveScreenshot(resultImagePath);

                // Copy the expected image into the results directory.
                const auto expectedFilename = result.Key + "-expected.png";
                fs::path expectedImagePath = testFileDirectory / subtest.Expected.Value.ValueString;
                fs::path expectedImageDest = testConfig.ResultsDirectory / "images" / expectedFilename;
                fs::copy_file(expectedImagePath, expectedImageDest, std::filesystem::copy_options::overwrite_existing);

                break;
            }
            case ResultType::Registers: {
                for(const auto registers = plip->GetCore()->GetCpuRegisters(); auto [ reg, value ] : registers) {
                    result.Actual.Value.ValueRegs.insert_or_assign(reg, value.ValueInt);
                }
                break;
            }
            case ResultType::None:
            default:
                throw std::runtime_error("BUG: Invalid expected result type was not caught early!");
        }

        if(unitTest.ShowResultsAsScreenshot && subtest.Expected.Type != ResultType::Image) {
            // Test requests that the result be shown as a screenshot.
            result.Actual.Value.ValueString = result.Key + "-actual.png";
            fs::path resultImagePath = testConfig.ResultsDirectory / "images" / result.Actual.Value.ValueString;
            mockVideo->SaveScreenshot(resultImagePath);
        }

        // Compare results.
        ResultComparator::CompareResults(testConfig, result);

        return result;
    }
};
