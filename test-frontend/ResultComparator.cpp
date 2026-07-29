// ResultComparator.cpp
//
// Compares the expected results vs. the actual results.

#include <cstdio>
#include <filesystem>
#include <format>
#include <vector>

#include <png.h>

#include "Models/FrontendConfig.h"
#include "ResultComparator.h"

namespace fs = std::filesystem;

namespace {
    struct ResultImage {
        uint32_t Width {};
        uint32_t Height {};
        std::vector<uint32_t> Pixels {};
    };
}

static ResultImage readPng(const std::string& filename) {
    png_image image = {
        .opaque = nullptr,
        .version = PNG_IMAGE_VERSION,
    };

    png_image_begin_read_from_file(&image, filename.c_str());
    if(image.warning_or_error >= 2) {
        // Ignore warnings; throw on error.
        throw std::runtime_error(std::format("Error reading screenshot '{}': {}", filename, image.message));
    }

    ResultImage result {
        .Width = image.width,
        .Height = image.height,
    };
    result.Pixels.resize(image.width * image.height);

    image.format = PNG_COLOR_TYPE_RGBA;
    const png_int_32 stride = PNG_IMAGE_ROW_STRIDE(image);

    // Note: I tried reading directly into result.Pixels, but that consistently resulted in overflows
    // and various other sorts of corruption. Instead we'll use an intermediate buffer and copy it
    // into the results.
    std::vector<uint8_t> buffer(PNG_IMAGE_SIZE(image));
    png_image_finish_read(&image, nullptr, std::data(buffer), stride, nullptr);

    if(image.warning_or_error >= 2) {
        // Ignore warnings; throw on error.
        throw std::runtime_error(std::format("Error reading screenshot '{}': {}", filename, image.message));
    }

    // Copy the buffer into the struct.
    result.Pixels.assign(buffer.begin(), buffer.end());

    return result;
}

void ResultComparator::CompareResults(const FrontendConfig& config, TestResult& results) {
    const auto resultDir = config.ResultsDirectory;

    switch(results.Expected().Type) {
        case ResultType::Image: {
            // HACK: fugly, fickle, and fragile
            const fs::path actual = resultDir / "images" / (results.Key + "-actual.png");
            const fs::path expected = resultDir / "images" / (results.Key + "-expected.png");

            results.Success = CompareImages(results, expected, actual);
            break;
        }
        case ResultType::Registers: {
            results.Success = CompareRegisters(results);
            break;
        }
        case ResultType::Memory: {
            results.Success = CompareMemory(results);
            break;
        }
        case ResultType::None:
        default:
            break;
    }
}

bool ResultComparator::CompareImages(TestResult& results, const std::string& expected, const std::string& actual) {
    // Decode both PNGs.
    ResultImage actualImage, expectedImage;
    try {
        expectedImage = readPng(expected);
    } catch(std::runtime_error& ex) {
        results.ComparisonError = std::format("Error loading expected image '{}': {}", expected, ex.what());
        return false;
    }

    try {
        actualImage = readPng(actual);
    } catch(std::runtime_error& ex) {
        results.ComparisonError = std::format("Error loading actual image '{}': {}", actual, ex.what());
        return false;
    }

    // Compare sizes.
    if(actualImage.Width != expectedImage.Width || actualImage.Height != expectedImage.Height) {
        results.ComparisonError = "Size mismatch between expected and actual images.";
        return false;
    }

    // Compare pixels.
    for(auto i = 0; i < actualImage.Pixels.size(); ++i) {
        if(expectedImage.Pixels[i] != actualImage.Pixels[i]) {
            return false;
        }
    }

    return true;
}

bool ResultComparator::CompareMemory(TestResult& results) {
    bool success = true;
    for(const auto [ addr, val ] : results.Expected().Value.ValueMem) {
        if(val.ReportOnly) continue;
        if(results.Actual.Value.ValueMem[addr].Value != val.Value) {
            results.MismatchedMemoryAddresses.insert(addr);
            success = false;
        }
    }

    return success;
}

bool ResultComparator::CompareRegisters(TestResult& results) {
    bool success = true;
    for(const auto& [ reg, val ] : results.Expected().Value.ValueRegs) {
        if(results.Actual.Value.ValueRegs[reg] != val) {
            results.MismatchedRegisters.insert(reg);
            success = false;
        }
    }

    return success;
}
