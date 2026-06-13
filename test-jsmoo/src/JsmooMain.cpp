// JsmooMain.cpp
//
// Entry point for the JSMoo-based CPU unit tests.

#include <iostream>
#include <map>
#include <vector>

#include "cxxopts.hpp"

const std::map<std::string, std::string> supportedCpus {
    { "6502", "MOS 6502" },
    { "2a03", "Ricoh 2A03/2A07" },
    { "sm83", "Sharp SM83" },
};

cxxopts::ParseResult ParseCmdLine(const int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);

        options.add_options()
            ("h,help", "shows this help screen and exits")
            ("c,cpu", "the CPU(s) that should be tested", cxxopts::value<std::vector<std::string>>())
        ;

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

int main(const int argc, char** argv) {
    const auto opts = ParseCmdLine(argc, argv);
    std::vector<std::string> testCpus {};

    if(opts["cpu"].count()) {
        testCpus = opts["cpu"].as<std::vector<std::string>>();
    } else {
        for(const auto& cpu : supportedCpus) {
            testCpus.push_back(cpu.first);
        }
    }

    // Invalid CPU detection.
    std::vector<std::string> invalidCpus {};
    for(const auto& cpu : testCpus) {
        if(!supportedCpus.count(cpu)) {
            invalidCpus.push_back(cpu);
        }
    }

    if(!invalidCpus.empty()) {
        std::cerr << "The following CPUs are not supported by this utility:" << std::endl << std::endl;
        for(const auto& cpu : invalidCpus) {
            std::cerr << "    * " << cpu << std::endl;
        }
        exit(1);
    }

    return 0;
}
