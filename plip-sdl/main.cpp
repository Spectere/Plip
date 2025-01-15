/* main.cpp
 *
 * The big, magical entry point.
 */

// ReSharper disable once CppUnusedIncludeDirective
// NOTE: Don't remove this. It may be required for some platforms.
#include <SDL3/SDL_main.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "cxxopts.hpp"
#include "PlipInstance.h"

#include "Config.h"
#include "Game.h"
#include "SDL/SdlAudio.h"
#include "SDL/SdlEvent.h"
#include "SDL/SdlWindow.h"

#include "Timer/TimerSdl.h"

std::vector<std::vector<std::string>> defaultConfig = {
        { "video", "scale"    , "1"  },
        { "video", "targetFps", "60" }
};

std::vector<std::vector<std::string>> intParamMapping = {
        { "scale", "video", "scale"     },
        { "fps"  , "video", "targetFps" }
};

cxxopts::ParseResult ParseCmdLine(const int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);

        options.positional_help("CORE FILENAME");

        options.add_options()
                ("core", "the core that should be used", cxxopts::value<std::string>())
                ("filename", "the path to the ROM", cxxopts::value<std::string>())
        ;

        options.add_options("General")
                ("h,help", "shows this help screen and exits")
                ("c,config", "specifies a config file", cxxopts::value<std::string>())
                ("l,list-cores", "shows a list of all supported cores and exits")
                ("V,version", "displays the version information and exits")
        ;

        options.add_options("Video")
                ( "f,fps", "sets the target frame rate", cxxopts::value<int>()->default_value("60"))
                ( "i,integer-scaling", "disallows fractional scaling during window resizes")
                ( "s,scale", "sets the initial window scaling", cxxopts::value<int>()->default_value("1"))
        ;

        options.parse_positional({"core", "filename" });

        auto result = options.parse(argc, argv);

        if(result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        return result;
    } catch(cxxopts::exceptions::incorrect_argument_type &ex) {
        std::cerr << "Invalid argument type (" << ex.what() << ")" << std::endl;
        exit(1);
    } catch(cxxopts::exceptions::missing_argument &ex) {
        std::cerr << ex.what() << std::endl;
        exit(1);
    } catch(cxxopts::exceptions::option_requires_argument &ex) {
        std::cerr << "Option requires argument (" << ex.what() << ")" << std::endl;
        exit(1);
    }
}

extern "C"
int main(int argc, char **argv) {
    auto opts = ParseCmdLine(argc, argv);
    auto version = Plip::PlipInstance::GetVersion();

    if(opts.count("version")) {
        std::cout << version << std::endl;
        return 0;
    }

    auto coreList = Plip::PlipInstance::GetSupportedCores();
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

    for(auto opt : defaultConfig) {
        config->SetValue(opt[0], opt[1], opt[2]);
    }

    if(opts["config"].count()) {
        const auto configFile = opts["config"].as<std::string>();
        if(!config->LoadFile(configFile)) {
            std::cerr << "Error opening config file: " << configFile << std::endl;
        }
    } else {
        if(std::filesystem::exists("plip.conf")) {
            config->LoadFile("plip.conf");
        }
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

    // Audio Settings
    const auto sampleRate = config->GetValue<int>("audio", "sample-rate", 48000);
    const auto bufferLength = config->GetValue<int>("audio", "buffer-length", 8192);

    auto audio = new PlipSdl::SdlAudio(sampleRate, bufferLength);

    // Video Settings
    auto videoScale = config->GetValue<int>("video", "scale", 1);
    auto integerScaling = config->GetValue<bool>("video", "integer-scaling", false);
    auto targetFps = config->GetValue<int>("video", "target-fps", 60);

    if(opts["integer-scaling"].count() > 0) {
        integerScaling = true;
    }

    if(opts["fps"].count() > 0) {
        targetFps = opts["fps"].as<int>();
    }

    auto window = new PlipSdl::SdlWindow(version, integerScaling);
    auto plip = new Plip::PlipInstance(window, audio);
    auto timer = new PlipSdl::TimerSdl();

    switch(plip->Load(coreTag, filename)) {
        case Plip::PlipError::FileNotFound:
            std::cout << "File not found (" << filename << ")!\n" << std::endl;
            return 1;
        case Plip::PlipError::InvalidCore:
            std::cout << "BUG: Core implemented improperly!\n" << std::endl;
            return 1;
        default:
            break;
    }

    auto input = plip->GetInput();
    auto event = new PlipSdl::SdlEvent(input);

    // Load inputs for the active core.
    std::string section = "input." + coreName;
    for(const auto&[id, definition] : input->GetInputList()) {
        auto key = config->GetValue(section, definition.GetDescription());
        if(key == config->empty) continue;

        event->AddDigitalBinding(id, key);
    }

    // Set the initial window scaling...
    window->SetScale(videoScale);

    // ...then go into the game loop.
    auto game = std::make_unique<PlipSdl::Game>(plip, event, window, timer, targetFps);
    game->Run();

    // fin.
    SDL_Quit();
    return 0;
}
