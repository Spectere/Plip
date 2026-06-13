// JsmooMain.cpp
//
// Entry point for the JSMoo-based CPU unit tests.

#include <filesystem>
#include <iostream>
#include <map>
#include <set>
#include <vector>

#include "cxxopts.hpp"

#include "Runner.h"
#include "RunnerSharpSm83.h"

namespace fs = std::filesystem;

const std::map<std::string, std::string> supportedCpus {
    { "6502", "MOS 6502" },
    { "2a03", "Ricoh 2A03/2A07" },
    { "sm83", "Sharp SM83" },
};

cxxopts::ParseResult ParseCmdLine(const int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);

        options.positional_help("CPU TESTS");

        options.add_options()
            ("cpu", "the CPU that should be tested", cxxopts::value<std::string>())
            ("tests", "one or more directories or files containing the tests to run", cxxopts::value<std::vector<std::string>>())
        ;

        options.add_options("General")
            ("h,help", "shows this help screen and exits")
            ("r,recursive", "scans directories recursively")
            ("t,threads", "the number of test threads (default = number of CPU cores)", cxxopts::value<int>())
        ;

        options.parse_positional({ "cpu", "tests" });

        auto result = options.parse(argc, argv);

        if(result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        return result;
    } catch(cxxopts::exceptions::no_such_option &ex) {
        std::cerr << argv[0] << ": " << ex.what() << std::endl;
        exit(1);
    } catch(cxxopts::exceptions::incorrect_argument_type &ex) {
        std::cerr << argv[0] << ": Invalid argument type (" << ex.what() << ")" << std::endl;
        exit(1);
    } catch(cxxopts::exceptions::missing_argument &ex) {
        std::cerr << argv[0] << ": " << ex.what() << std::endl;
        exit(1);
    } catch(cxxopts::exceptions::option_requires_argument &ex) {
        std::cerr << argv[0] << ": Option requires argument (" << ex.what() << ")" << std::endl;
        exit(1);
    }
}

bool IsTestFile(const fs::directory_entry& entry) {
    return entry.path().has_extension() && entry.path().extension() == ".json";
}

void ShowReport(const std::set<RunnerTestResult>& results) {
    const auto totalTests = results.size();

    std::set<RunnerTestResult> failedTests {};
    for(const auto &result : results) {
        if(!result.Success()) failedTests.insert(result);
    }

    if(!failedTests.empty()) {
        for(const auto &result : failedTests) {
            std::cout << result.Key << ": " << std::endl;

            for(const auto &reg : result.RegisterMisses) {
                std::cout << "register " << reg.Register << " (expected: " << reg.Expected << "; actual: " << reg.Actual << ")" << std::endl;
            }

            for(const auto &mem : result.MemoryMisses) {
                std::cout << "memory [addr: " << mem.Address << "] (expected: " << mem.Expected << "; actual: " << mem.Actual << ")" << std::endl;
            }

            std::cout << std::endl;
        }
    }

    std::cout << "summary: " << totalTests - failedTests.size() << " / " << totalTests << " tests passed" << std::endl;
    if(!failedTests.empty()) {
        std::cout << " FAILED: " << failedTests.size() << " tests" << std::endl;
    }
}

std::set<std::string> FindTestFiles(const std::vector<std::string>& paths, const bool recursive) {
    std::set<std::string> testFiles {};

    for(const auto &path : paths) {
        // Should already be filtered out, but better safe than sorry...
        if(!fs::exists(path)) continue;

        // Just in case someone decides to point this to /dev or something.
        if(!(fs::is_directory(path) || fs::is_regular_file(path))) continue;

        if(fs::is_regular_file(path)) {
            testFiles.insert(path);
            continue;
        }

        // Directory scanning.
        if(recursive) {  // urgggg, why
            for(const auto &entry : fs::recursive_directory_iterator(path)) {
                if(IsTestFile(entry)) {
                    testFiles.insert(entry.path());
                }
            }
        } else {
            for(const auto &entry : fs::directory_iterator(path)) {
                if(IsTestFile(entry)) {
                    testFiles.insert(entry.path());
                }
            }
        }
    }

    return testFiles;
}

int main(const int argc, char** argv) {
    const auto opts = ParseCmdLine(argc, argv);

    std::string testCpu {};
    std::vector<std::string> testPaths {};

    if(!opts.count("cpu") || !opts.count("tests")) {
        std::cerr << "The CPU and test directory/files must be specified!\n\n"
                  << "Please see the usage information (" << argv[0] << " -h) for more information." << std::endl;
        return 1;
    }

    testCpu = opts["cpu"].as<std::string>();
    testPaths = opts["tests"].as<std::vector<std::string>>();
    const auto testRecursive = opts["recursive"].as<bool>();

    auto threads = 0;
    if(opts.count("threads")) {
        threads = opts["threads"].as<int>();
    }

    // Invalid CPU detection.
    if(!supportedCpus.count(testCpu)) {
        std::cerr << "The CPU '" << testCpu << "' is not supported by this utility." << std::endl;
        return 1;
    }

    // Invalid path detection.
    std::vector<std::string> invalidPaths {};
    for(const auto &path : testPaths) {
        if(!fs::exists(path)) {
            invalidPaths.push_back(path);
        }
    }

    if(!invalidPaths.empty()) {
        std::cerr << "The following paths could not be found:\n" << std::endl;
        for(const auto &path : invalidPaths) {
            std::cerr << "    * " << path << std::endl;
        }
        return 1;
    }

    // Find test files, then dispatch them to the appropriate test harness.
    const auto testFiles = FindTestFiles(testPaths, testRecursive);
    if(testFiles.empty()) {
        std::cerr << "No test files found." << std::endl;
        return 1;
    }

    // TODO: This could get ugly really fast. This is fine for testing, but find a better approach.
    std::set<RunnerTestResult> results {};
    if(testCpu == "sm83") {
        Runner<RunnerSharpSm83> runner;
        results = runner.RunTests(testFiles, threads);
    }

    // Report results to user.
    ShowReport(results);

    return 0;
}
