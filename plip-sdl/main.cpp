/* main.cpp
 *
 * The big, magical entry point.
 */

#include <iostream>

#include "cxxopts.hpp"
#include "Plip.h"

#include "SdlEvent.h"
#include "SdlWindow.h"

#ifdef UNIX
#include "TimerPosix.h"
#else
#include "TimerSdl.h"
#endif

void gameLoop(Plip::Plip *plip, PlipSdl::Timer *timer) {
}

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

#include "PlipMemoryMap.h"
#include "PlipMemoryRam.h"
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

    /*
    auto wnd = new PlipSdl::SdlWindow(opts["scale"].as<int>(), version);
    auto event = new PlipSdl::SdlEvent();
    auto plip = new Plip::Plip(event, wnd);

#ifdef UNIX
    auto timer = new PlipSdl::TimerPosix();
#else
    auto timer = new PlipSdl::TimerSdl();
#endif

    gameLoop(plip, timer);
    */

    auto memMap = new Plip::PlipMemoryMap();
    auto ram1 = new Plip::PlipMemoryRam(0x2000);
    auto ram2 = new Plip::PlipMemoryRam(0x2000);
    auto ram3 = new Plip::PlipMemoryRam(0x1000);
    auto ram4 = new Plip::PlipMemoryRam(0x1000);
    auto ram5 = new Plip::PlipMemoryRam(0x2000);
    auto ram6 = new Plip::PlipMemoryRam(0x2000);
    auto ram7 = new Plip::PlipMemoryRam(0x2000);
    auto ram8 = new Plip::PlipMemoryRam(0x4000);
    memMap->AddBlock(ram1); // 0x0000 - 0x1FFF
    memMap->AddBlock(ram2); // 0x2000 - 0x3FFF
    memMap->AddBlock(ram3); // 0x4000 - 0x4FFF
    memMap->AddBlock(ram4); // 0x5000 - 0x5FFF
    memMap->AddBlock(ram5); // 0x6000 - 0x7FFF
    memMap->AddBlock(ram6); // 0x8000 - 0x9FFF
    memMap->AddBlock(ram7); // 0xA000 - 0xBFFF
    memMap->AddBlock(ram8); // 0xC000 - 0xFFFF

    memMap->AssignBlock(ram4, 0x1000, 0x0000, 0x1000);
    memMap->UnassignBlock(0x4000, 0x800);

    std::cout << "Map length: " << memMap->GetLength() << std::endl;

    return 0;
}
