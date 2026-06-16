// Runner.h
//
// A multithreaded test dispatcher.

#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>

#include "IRunner.h"
#include "PlipEmulationException.h"
#include "RunnerCpu.h"
#include "RunnerTest.h"
#include "RunnerTestResult.h"

using namespace nlohmann;

template<class RunnerCpuType>
class Runner : public IRunner {
    static_assert(std::is_base_of_v<RunnerCpu, RunnerCpuType>, "RunnerCpuType must be a subclass of RunnerCpu");

public:
    std::set<RunnerTestResult> RunTests(const std::set<std::string>& tests) override {
        using namespace std::chrono;

        const auto parseStart = steady_clock::now();
        for(const auto &file : tests) {
            LoadTestCollection(file);
        }
        const auto parseEnd = steady_clock::now();
        const auto parseTime = parseEnd - parseStart;
        std::cout << "Loaded " << m_tests.size() << " tests in "
                  << static_cast<float>(duration_cast<milliseconds>(parseTime).count()) / 1000.0f
                  << " seconds." << std::endl;

        const auto testStart = steady_clock::now();
        std::set<RunnerTestResult> allResults {};
        for(const auto &test : m_tests) {
            try {
                allResults.insert(PerformTest(test));
            } catch(Plip::PlipEmulationException& ex) {
                RunnerTestResult result {};
                result.Key = FormatKey(test);
                result.Filename = test.Filename;
                result.Skipped = ex.SkipsUnitTest;

                if(result.Skipped)
                    result.SkipReason = "Skippable Error: " + std::string(ex.what());
                else
                    result.ExceptionsThrown.emplace_back("PlipEmulationException: " + std::string(ex.what()));

                allResults.insert(result);
            } catch(std::exception& ex) {
                RunnerTestResult result {};
                result.Key = FormatKey(test);
                result.ExceptionsThrown.emplace_back(ex.what());

                allResults.insert(result);
            }
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
        result.Filename = test.Filename;

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

        // Verify PC.
        if(cpu->GetPc() != test.FinalState.ProgramCounter) {
            result.RegisterMisses.push_back({ "PC", test.FinalState.ProgramCounter, cpu->GetPc() });
        }

        // Verify memory.
        for(const auto [ addr, expected ] : test.FinalState.Memory) {
            if(const auto actual = cpu->ReadMemory(addr); actual != expected) {
                result.MemoryMisses.push_back({ addr, expected, actual });
            }
        }

        return result;
    }
};
