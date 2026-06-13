// Runner.h
//
// A multithreaded test dispatcher.

#pragma once

#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>

#include "PlipEmulationException.h"
#include "RunnerCpu.h"
#include "RunnerTest.h"
#include "RunnerTestResult.h"

using namespace nlohmann;

template<class RunnerCpuType>
class Runner {
    static_assert(std::is_base_of_v<RunnerCpu, RunnerCpuType>, "RunnerCpuType must be a subclass of RunnerCpu");

public:
    std::set<RunnerTestResult> RunTests(const std::set<std::string>& tests, unsigned int threads) {
        using namespace std::chrono;

        if(!threads) {
            // Get hardware thread count.
            if(const auto detectedThreads = std::thread::hardware_concurrency(); detectedThreads > 0) {
                threads = detectedThreads;
            } else {
                // thread::hardware_concurrency() returned 0. Fallback to 4.
                threads = 4;
            }
        }

        const auto parseStart = steady_clock::now();
        for(const auto &file : tests) {
            LoadTestCollection(file);
        }
        const auto parseEnd = steady_clock::now();
        const auto parseTime = parseEnd - parseStart;
        std::cout << "Loaded " << m_tests.size() << " tests in "
                  << static_cast<float>(duration_cast<milliseconds>(parseTime).count()) / 1000.0f
                  << " seconds." << std::endl;

        std::cout << "Running tests using " << threads << " threads..." << std::endl;

        const auto testStart = steady_clock::now();
        std::set<RunnerTestResult> allResults {};
        std::mutex allResultsMutex;
        std::atomic<size_t> nextTestIdx { 0 };

        auto worker = [&] {
            std::vector<RunnerTestResult> workerResults;
            size_t thisTestIdx;

            while((thisTestIdx = nextTestIdx.fetch_add(1, std::memory_order_relaxed)) < m_tests.size()) {
                try {
                    workerResults.push_back(PerformTest(m_tests[thisTestIdx]));
                } catch(Plip::PlipEmulationException& ex) {
                    RunnerTestResult result {};
                    result.Key = FormatKey(m_tests[thisTestIdx]);
                    result.Skipped = true;
                    result.SkipReason = "PlipEmulationException: " + std::string(ex.what());
                    workerResults.push_back(result);
                }
            }

            std::lock_guard lock(allResultsMutex);
            for(auto& r : workerResults) {
                allResults.insert(std::move(r));
            }
        };

        std::vector<std::thread> threadPool;
        threadPool.reserve(threads);

        for(unsigned t = 0; t < threads; ++t) {
            threadPool.emplace_back(worker);
        }

        for(auto& thread : threadPool) {
            thread.join();
        }

        const auto testEnd = steady_clock::now();
        const auto testTime = testEnd - testStart;
        std::cout << "All tests completed in "
                  << static_cast<float>(duration_cast<milliseconds>(testTime).count()) / 1000.0f
                  << " seconds." << std::endl;

        return allResults;
    }

private:
    std::vector<RunnerTest> m_tests;

    static std::vector<RunnerTestCycle> ConvertTestCycles(const json& def) {
        std::vector<RunnerTestCycle> cycles {};

        for(const auto &cycleDef : def) {
            RunnerTestCycle cycle {};

            cycle.Address = cycleDef[0].get<uint32_t>();
            cycle.Value = cycleDef[1].get<uint8_t>();

            auto access = cycleDef[2].get<std::string>();
            cycle.MemoryRead = access[0] == 'r';
            cycle.MemoryWrite = access[1] == 'w';
            cycle.MemoryRequest = access[2] == 'm';

            cycles.push_back(cycle);
        }

        return cycles;
    }

    static std::vector<RunnerTestMemory> ConvertTestMemory(const json& def) {
        std::vector<RunnerTestMemory> mem {};

        for(const auto &memDef : def) {
            RunnerTestMemory b {};

            b.Address = memDef[0].get<uint32_t>();
            b.Value = memDef[1].get<uint8_t>();

            mem.push_back(b);
        }

        return mem;
    }

    static RunnerTestState ConvertTestState(const json& def) {
        RunnerTestState state {};

        state.Memory = ConvertTestMemory(def["ram"]);
        state.ProgramCounter = def["pc"].get<uint32_t>();

        for(const auto &obj : def.items()) {
            if(const auto &key = obj.key(); key == "ram" || key == "pc") {
                continue;  // Already handled.
            }

            state.Registers.insert({ obj.key(), obj.value().get<uint64_t>() });
        }

        return state;
    }

    static RunnerTest ConvertTestDefinition(const std::string& filename, const json& def) {
        RunnerTest test {};

        test.Filename = filename;
        test.TestName = def["name"].get<std::string>();
        test.Cycles = ConvertTestCycles(def["cycles"]);
        test.InitialState = ConvertTestState(def["initial"]);
        test.FinalState = ConvertTestState(def["final"]);

        return test;
    }

    std::string FormatKey(const RunnerTest& test) {
        return test.Filename + " [" + test.TestName + "]";
    }

    void LoadTestCollection(const std::string& filename) {
        std::ifstream file(filename);
        const auto data = json::parse(file);

        for(const auto &test : data) {
            m_tests.push_back(ConvertTestDefinition(filename, test));
        }

        file.close();
    }

    RunnerTestResult PerformTest(const RunnerTest& test) {
        RunnerTestResult result {};
        const auto cpu = std::make_unique<RunnerCpuType>();

        result.Key = FormatKey(test);

        // Set initial memory state.
        for(const auto [ addr, val ] : test.InitialState.Memory) {
            cpu->WriteMemory(addr, val);
        }

        // Set initial CPU state.
        cpu->SetInitialState(test.InitialState);

        // Clock CPU for the requested number of cycles.
        const auto cycleCount = test.Cycles.size();
        for(auto i = 0; i < cycleCount; ++i) {
            cpu->Step();
        }

        // Check final state and submit report.
        result.RegisterMisses = cpu->CompareState(test.FinalState);

        for(const auto [ addr, expected ] : test.FinalState.Memory) {
            if(const auto actual = cpu->ReadMemory(addr); actual != expected) {
                result.MemoryMisses.push_back({ addr, expected, actual });
            }
        }

        return result;
    }
};
