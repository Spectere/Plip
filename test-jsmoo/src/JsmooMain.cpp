// JsmooMain.cpp
//
// Entry point for the JSMoo-based CPU unit tests.

#include <filesystem>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "cxxopts.hpp"

#include "Runner.h"
#include "RunnerSharpSm83.h"
#include "TestableCpu.h"

namespace fs = std::filesystem;

//{ "6502", "MOS 6502" },
//{ "2a03", "Ricoh 2A03/2A07" },
const std::map<std::string, TestableCpu> supportedCpus {
    { "sm83", {
        "Sharp SM83",
        []{ return std::make_unique<Runner<RunnerSharpSm83>>(); }
    }},
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

int ShowReport(const std::set<RunnerTestResult>& results) {
    const auto totalTests = results.size();

    std::set<RunnerTestResult> failedTests {};
    for(const auto &result : results) {
        if(!result.Success()) failedTests.insert(result);
    }

    std::set<RunnerTestResult> skippedTests {};
    for(const auto &result : results) {
        if(result.Skipped) skippedTests.insert(result);
    }

    if(!failedTests.empty()) {
        for(const auto &result : failedTests) {
            std::cerr << result.Key << ": " << std::endl;

            for(const auto &reg : result.RegisterMisses) {
                std::cerr << "register " << reg.Register << " (expected: " << reg.Expected << "; actual: " << reg.Actual << ")" << std::endl;
            }

            for(const auto &mem : result.MemoryMisses) {
                std::cerr << "memory [addr: " << mem.Address << "] (expected: " << static_cast<uint16_t>(mem.Expected) << "; actual: " << static_cast<uint16_t>(mem.Actual) << ")" << std::endl;
            }

            for(const auto &ex : result.ExceptionsThrown) {
                std::cerr << "exception thrown: " << ex << std::endl;
            }

            std::cerr << std::endl;
        }
    }

    std::cout << "summary: " << totalTests - failedTests.size() - skippedTests.size() << " / " << totalTests << " tests passed" << std::endl;
    if(!failedTests.empty()) {
        std::cerr << " FAILED: " << failedTests.size() << " tests" << std::endl;
    }
    if(!skippedTests.empty()) {
        std::cout << "SKIPPED: " << skippedTests.size() << " tests" << std::endl;
    }

    return failedTests.empty() ? 0 : 1;
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

    // Dispatch, gather results, and report them to the user.
    const auto runner = supportedCpus.at(testCpu).Create();
    const auto results = runner->RunTests(testFiles);
    return ShowReport(results);
}
