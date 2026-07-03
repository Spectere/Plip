// FrontendMain.cpp
//
// A tester that's capable of visual comparisons between Plip's output and reference PNGs.

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

#include <nlohmann/json.hpp>

#include "Models/Results.h"
#include "Models/FrontendConfig.h"
#include "Models/UnitTest.h"
#include "PlipRunner.h"
#include "ResultWriter.h"

namespace fs = std::filesystem;
using namespace nlohmann;

bool IsTestFile(const fs::directory_entry& entry) {
    return entry.path().has_extension() && entry.path().extension() == ".json";
}

std::set<std::string> ParseCmdLine(const int argc, char **argv) {
    if(argc == 1) {
        // No filename given.
        std::cout << "usage: " << argv[0] << " [paths/files]" << std::endl;
        exit(1);
    }

    std::set<std::string> validFiles {};
    std::map<std::string, std::string> invalidFiles {};
    for(auto i = 1; i < argc; ++i) {
        const auto candidate = std::string(argv[i]);

        if(!fs::exists(candidate)) {
            invalidFiles.insert({ candidate, "not found" });
            continue;
        }

        if(fs::is_regular_file(candidate)) {
            // Regular file. Add it to the list and continue;
            validFiles.insert(candidate);
            continue;
        }

        if(fs::is_directory(candidate)) {
            // Directory. Scan it recursively and add contents (if applicable).
            for(const auto &entry : fs::recursive_directory_iterator(candidate)) {
                if(IsTestFile(entry)) {
                    validFiles.insert(entry.path());
                }
            }
            continue;
        }

        invalidFiles.insert({ candidate, "not a regular file or directory" });
    }

    if(!invalidFiles.empty()) {
        std::cerr << "Invalid files found:\n\n";
        for(const auto &[ file, reason ] : invalidFiles) {
            std::cerr << " * " << file << " : " << reason << std::endl;
        }
    }

    return validFiles;
}

StopEvent LoadStopEvent(const std::string& filename, const json& def, const FrontendConfig& config) {
    StopEvent ev {};

    if(const auto type = def["type"].get<std::string>(); type == "opcode") {
        ev.Type = StopType::Opcode;
        ev.Value.ValueUInt = def["valueUInt"].get<uint64_t>();
    } else if(type == "cycles") {
        ev.Type = StopType::Cycles;
        ev.Value.ValueUInt = def["valueUInt"].get<uint64_t>();
    } else {
        throw std::runtime_error("Invalid stop type in file: " + filename);
    }

    return ev;
}

Results LoadExpectedResult(const std::string& filename, const json& def, const FrontendConfig& config) {
    Results expected {};

    if(const auto type = def["type"]; type == "registers") {
        expected.Type = ResultType::Registers;
        for(const auto &reg : def["valueRegs"].items()) {
            expected.Value.ValueRegs.insert({ reg.key(), reg.value().get<uint64_t>() });
        }
    } else if(type == "image") {
        expected.Type = ResultType::Image;
        expected.Value.ValueString = def["valueString"].get<std::string>();
    } else {
        throw std::runtime_error("Invalid expected result type in file: " + filename);
    }

    return expected;
}

std::vector<Subtest> LoadSubtests(const std::string& filename, const json& def, const FrontendConfig& config) {
    std::vector<Subtest> subtests {};

    for(const auto &test : def) {
        Subtest t {};

        t.Name = test["name"].get<std::string>();
        t.Config = test["config"].get<std::unordered_map<std::string, std::string>>();
        t.Expected = LoadExpectedResult(filename, test["expected"], config);

        subtests.push_back(t);
    }

    return subtests;
}

std::string FormatKey(const UnitTest& test) {
    return test.Core + "-" + test.Suite + "-" + test.Name;
}

UnitTest LoadTest(const std::string& filename, const json& def, const FrontendConfig& config) {
    UnitTest test {};

    test.Filename = filename;
    test.Name = def["name"].get<std::string>();
    test.Suite = def["suite"].get<std::string>();
    test.Core = def["core"].get<std::string>();
    test.Rom = def["rom"].get<std::string>();
    test.Stop = LoadStopEvent(filename, def["stop"], config);
    test.Subtests = LoadSubtests(filename, def["tests"], config);

    test.Key = FormatKey(test);

    return test;
}

int main(const int argc, char** argv) {
    using namespace std::chrono;

    const auto testFiles = ParseCmdLine(argc, argv);

    // Load the config file.
    std::ifstream configFile("plip-test-frontend.conf");
    const auto json = json::parse(configFile);

    const auto resultsPath = fs::path(json["ResultsDirectory"].get<std::string>());
    const auto assetsPath = fs::path(json["AssetsDirectory"].get<std::string>());

    uint64_t threads = 0;
    if(json.contains("Threads")) {
        threads = json["Threads"].get<uint64_t>();
    }

    if(threads == 0) {
        threads = std::thread::hardware_concurrency();

        if(threads == 0) {
            // thread::hardware_concurrency is not guaranteed to return a result.
            // 4 is a reasonable fallback for modern CPUs.
            threads = 4;
        }
    }

    // Create the results directories if it doesn't exist.
    if(!fs::exists(resultsPath) && !fs::create_directories(resultsPath)) {
        std::cerr << "An error occurred while creating the results directory." << std::endl;
        exit(1);
    }

    if(!fs::exists(resultsPath / "images") && !fs::create_directories(resultsPath / "images")) {
        std::cerr << "An error occurred while creating the images directory." << std::endl;
        exit(1);
    }

    const FrontendConfig testConfig {
        .ResultsDirectory = fs::canonical(resultsPath),
        .AssetsDirectory = fs::canonical(assetsPath),
        .Threads = threads,
    };
    configFile.close();

    // Load the tests.
    std::set<UnitTest> unitTests {};
    for(const auto& file : testFiles) {
        std::ifstream f(file);
        unitTests.insert(LoadTest(file, json::parse(f), testConfig));
    }

    if(!fs::exists(testConfig.ResultsDirectory)) {
        fs::create_directory(testConfig.ResultsDirectory);
    }

    // Count the total subtests.
    size_t testCount {};
    for(const auto& test : unitTests) {
        testCount += test.Subtests.size();
    }

    // Let's go!
    std::cout << "Running " << testCount << " tests with " << testConfig.Threads << " threads..." << std::endl;
    const auto start = steady_clock::now();
    const auto results = PlipRunner::RunTests(testConfig, unitTests);
    const auto end = steady_clock::now();
    const auto time = end - start;
    const auto timeSec = static_cast<double>(duration_cast<milliseconds>(time).count()) / 1000.0f;

    // Tally up failures.
    size_t errorCount {};
    for(const auto& result : results) {
        if(!result.Success) ++errorCount;
    }

    if(errorCount) std::cerr << " === " << errorCount << " FAILED" << " === " << std::endl;
    std::cout << " === " << testCount << " tests executed in " << timeSec << "s ===" << std::endl;

    // Write results to an HTML file.
    ResultWriter::WriteHtml(testConfig.ResultsDirectory / "index.html", results);

    return (errorCount > 0) ? 1 : 0;
}
