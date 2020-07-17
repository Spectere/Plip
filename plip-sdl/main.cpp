/* main.cpp
 *
 * The big, magical entry point.
 */

#include <iostream>

#include "cxxopts.hpp"
#include "Plip.h"

#include "SdlEvent.h"
#include "SdlWindow.h"

cxxopts::ParseResult parseCmdLine(int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);

        options.positional_help("CORE FILENAME")
                .show_positional_help();

        options.add_options(cxxopts::hidden_group)
                ("c,core", "the core that should be used", cxxopts::value<std::string>())
                ("f,filename", "the path to the ROM", cxxopts::value<std::string>())
                ("positional", "", cxxopts::value<std::vector<std::string>>())
        ;

        options.add_options()
                ("h,help", "shows this help screen and exits")
                ("l,list-cores", "shows a list of all supported cores and exits")
                ("V,version", "displays the version information and exits");

        options.add_options("Video")
                ("s,scale", "sets the default window scaling", cxxopts::value<int>()->default_value("1"));

        options.parse_positional({"core", "filename", "positional"});

        auto result = options.parse(argc, argv);

        if(result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        return result;
    } catch(cxxopts::argument_incorrect_type &ex) {
        std::cerr << "Invalid argument type (" << ex.what() << ")" << std::endl;
        exit(1);
    } catch(cxxopts::missing_argument_exception &ex) {
        std::cerr << ex.what() << std::endl;
        exit(1);
    } catch(cxxopts::OptionException &ex) {
        std::cerr << "Error parsing arguments (" << ex.what() << ")" << std::endl;
        exit(1);
    }
}

int main(int argc, char **argv) {
    auto opts = parseCmdLine(argc, argv);
    auto version = Plip::Plip::GetVersion();

    if(opts.count("version")) {
        std::cout << version << std::endl;
        return 0;
    }

    if(!opts.count("core") || !opts.count("filename")) {
        std::cerr << "The name of the core and the filename must be specified!\n\n"
                  << "Please see the usage information (" << argv[0] << " -h) for more information." << std::endl;
        return 1;
    }

    auto wnd = new PlipSdl::SdlWindow(opts["scale"].as<int>(), version);
    auto event = new PlipSdl::SdlEvent();

    return 0;
}
