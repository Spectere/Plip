// FrontendConfig.h
//
// The config file model for plip-test-frontend.

#pragma once

#include <filesystem>

struct FrontendConfig {
    std::filesystem::path ResultsDirectory {};
    std::filesystem::path AssetsDirectory {};
};
