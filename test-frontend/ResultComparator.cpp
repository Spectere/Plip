// ResultComparator.cpp
//
// Compares the expected results vs. the actual results.

#include <filesystem>

#include "lodepng.h"

#include "Models/FrontendConfig.h"
#include "ResultComparator.h"

namespace fs = std::filesystem;

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
        case ResultType::None:
        default:
            break;
    }
}

bool ResultComparator::CompareImages(TestResult& results, const std::string& expected, const std::string& actual) {
    // Decode both PNGs.
    std::vector<unsigned char> expectedBuffer;
    std::vector<unsigned char> expectedImage;
    unsigned int expectedWidth, expectedHeight;
    if(lodepng::load_file(expectedBuffer, expected)) {
        results.ComparisonError = "Unable to open the expected image for comparison.";
        return false;
    }
    lodepng::decode(expectedImage, expectedWidth, expectedHeight, expectedBuffer);

    std::vector<unsigned char> actualBuffer;
    std::vector<unsigned char> actualImage;
    unsigned int actualWidth, actualHeight;
    if(lodepng::load_file(actualBuffer, actual)) {
        results.ComparisonError = "Unable to open the actual image for comparison.";
        return false;
    }
    lodepng::decode(actualImage, actualWidth, actualHeight, actualBuffer);

    // Compare sizes.
    if(actualWidth != expectedWidth || actualHeight != expectedHeight) {
        results.ComparisonError = "Size mismatch between expected and actual images.";
        return false;
    }

    // Compare pixels (note: LodePNG decodes to RGBA).
    for(auto i = 0; i < actualWidth * actualHeight * 4; ++i) {
        if(expectedImage[i] != actualImage[i]) {
            return false;
        }
    }

    return true;
}

bool ResultComparator::CompareRegisters(TestResult& results) {
    bool success = true;
    for(const auto reg : results.Expected().Value.ValueRegs) {
        if(results.Actual.Value.ValueRegs[reg.first] != reg.second) {
            results.MismatchedRegisters.insert(reg.first);
            success = false;
        }
    }

    return success;
}
