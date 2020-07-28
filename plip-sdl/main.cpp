/* main.cpp
 *
 * The big, magical entry point.
 */

#include <iostream>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#include "Plip.h"

#include "Config.h"
#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"

#ifdef UNIX
#include "Timer/TimerPosix.h"
#else
#include "Timer/TimerSdl.h"
#endif

std::vector<std::vector<std::string>> defaultConfig = {
        { "video", "scale"    , "1"  },
        { "video", "targetFps", "60" }
};

std::vector<std::vector<std::string>> intParamMapping = {
        { "scale", "video", "scale"     },
        { "fps"  , "video", "targetFps" }
};

void gameLoop(Plip::Plip *plip, PlipSdl::Config *config, PlipSdl::Timer *timer) {
    auto input = plip->GetInput();
    auto video = plip->GetVideo();

    auto event = new PlipSdl::SdlEvent(input);

    auto targetFps = config->GetValue<int>("video", "targetFps");
    auto frameTime = 1000000000 / targetFps;

    auto running = true;
    while(running) {
        timer->StopwatchStart();

        if(event->ProcessEvents() == PlipSdl::SdlUiEvent::Quit)
            running = false;

        auto time = timer->StopwatchStop();
        auto delay = frameTime - time;
        while(delay < 0)
            delay += frameTime;

        timer->Nanosleep(delay);
    }
}

cxxopts::ParseResult parseCmdLine(int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);

        options.positional_help("CORE FILENAME")
                .show_positional_help();

        options.add_options(cxxopts::hidden_group)
                ("core", "the core that should be used", cxxopts::value<std::string>())
                ("filename", "the path to the ROM", cxxopts::value<std::string>())
                ("positional", "", cxxopts::value<std::vector<std::string>>())
        ;

        options.add_options()
                ("h,help", "shows this help screen and exits")
                ("c,config", "specifies a config file", cxxopts::value<std::string>())
                ("l,list-cores", "shows a list of all supported cores and exits")
                ("V,version", "displays the version information and exits")
        ;

        options.add_options("Video")
                ( "f,fps", "sets the target frame rate", cxxopts::value<int>()->default_value("60"))
                ( "s,scale", "sets the default window scaling", cxxopts::value<int>()->default_value("1"))
        ;

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

    auto coreList = Plip::Plip::GetSupportedCores();
    if(opts["list-cores"].count()) {
        std::cout << "Supported cores:\n\n";
        std::cout << "    Name    |    Description\n";
        std::cout << "--------------------------------\n";

        for(auto core : coreList) {
            std::string name = core.name;
            name.append(10 - name.length(), ' ');  // Pad string.
            std::cout << " " << name << " | " << core.description << '\n';
        }

        std::cout << std::endl;
        return 0;
    }

    if(!opts.count("core") || !opts.count("filename")) {
        std::cerr << "The name of the core and the filename must be specified!\n\n"
                  << "Please see the usage information (" << argv[0] << " -h) for more information." << std::endl;
        return 1;
    }

    SDL_Init(0);

    auto config = new PlipSdl::Config();

    for(auto opt : defaultConfig)
        config->SetValue(opt[0], opt[1], opt[2]);

    if(opts["config"].count()) {
        auto configFile = opts["config"].as<std::string>();
        if(!config->LoadFile(configFile))
            std::cerr << "Error opening config file: " << configFile << std::endl;
    }

    // Check for core name validity.
    auto found = false;
    auto coreName = opts["core"].as<std::string>();
    auto filename = opts["filename"].as<std::string>();
    Plip::PlipValidCore coreTag;
    for(auto core : coreList) {
        if(core.name != coreName) continue;

        found = true;
        coreTag = core.descriptor;
        break;
    }

    if(!found) {
        std::cout << "Invalid core specified (" << coreName << ").\n\n";
        std::cout << "Please check the core list (" << argv[0] << " -l) for valid entries." << std::endl;
        return 1;
    }

    for(auto param : intParamMapping) {
        if(opts[param[0]].count()) {
            config->SetValue(
                    param[1], param[2],
                    std::to_string(opts[param[0]].as<int>())
            );
        }
    }

    auto videoScale = config->GetValue<int>("video", "scale");

    auto wnd = new PlipSdl::SdlWindow(videoScale, version);
    auto plip = new Plip::Plip(wnd);

#ifdef UNIX
    auto timer = new PlipSdl::TimerPosix();
#else
    auto timer = new PlipSdl::TimerSdl();
#endif

    auto result = plip->Load(coreTag, filename);
    switch(result) {
        case Plip::PlipError::FileNotFound:
            std::cout << "File not found (" << filename << ")!\n" << std::endl;
            return 1;
        case Plip::PlipError::InvalidCore:
            std::cout << "BUG: Core implemented improperly!\n" << std::endl;
            return 1;
        default:
            break;
    }

    gameLoop(plip, config, timer);

    SDL_Quit();
    return 0;
}
