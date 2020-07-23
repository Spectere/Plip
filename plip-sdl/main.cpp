/* main.cpp
 *
 * The big, magical entry point.
 */

#include <iostream>
#include <string>

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

void gameLoop(Plip::Plip *plip, PlipSdl::Config *config, PlipSdl::Timer *timer) {
    auto input = plip->GetInput();
    auto video = plip->GetVideo();

    auto event = new PlipSdl::SdlEvent(input);

    auto targetFps = std::stoi(config->GetValue("video", "targetFps"));
    auto frameTime = 1000000000 / targetFps;

    std::cout << targetFps << "hz (" << frameTime << "ns)" << std::endl;

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

    if(!opts.count("core") || !opts.count("filename")) {
        std::cerr << "The name of the core and the filename must be specified!\n\n"
                  << "Please see the usage information (" << argv[0] << " -h) for more information." << std::endl;
        return 1;
    }

    SDL_Init(0);

    // TODO: Make the config handler less of a mess. :|
    auto config = new PlipSdl::Config();
    config->SetValue("video", "scale", "1");
    config->SetValue("video", "targetFps", "60");

    if(opts["config"].count()) {
        auto configFile = opts["config"].as<std::string>();
        if(!config->LoadFile(configFile))
            std::cerr << "Error opening config file: " << configFile << std::endl;
    }

    if(opts["scale"].count())
        config->SetValue("video", "scale", std::to_string(opts["scale"].as<int>()));
    if(opts["fps"].count())
        config->SetValue("video", "targetFps", std::to_string(opts["fps"].as<int>()));

    auto videoScale = std::stoi(config->GetValue("video", "scale"));

    auto wnd = new PlipSdl::SdlWindow(videoScale, version);
    auto plip = new Plip::Plip(wnd);

#ifdef UNIX
    auto timer = new PlipSdl::TimerPosix();
#else
    auto timer = new PlipSdl::TimerSdl();
#endif

    gameLoop(plip, config, timer);

    SDL_Quit();
    return 0;
}
