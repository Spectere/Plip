/* PlipIo.cpp
 *
 * Standardized functions for dealing with file I/O.
 */

#include <filesystem>

#include "PlipIo.h"

namespace Plip {
    bool PlipIo::FileExists(const std::string &path) {
        std::ifstream file(path.c_str());
        bool success = file.good();
        if(success) file.close();
        return success;
    }

    uintmax_t PlipIo::GetSize(const std::string &path) {
        namespace fs = std::filesystem;
        return fs::file_size(fs::path(path));
    }

    std::ifstream PlipIo::LoadFile(const std::string &path) {
        return std::ifstream(path.c_str());
    }

    std::vector<char> PlipIo::ReadFile(std::ifstream &file, uintmax_t size) {
        std::vector<char> data(size);

        file.seekg(std::istream::beg);
        file.read(data.data(), data.size());
        return data;
    }

    std::vector<char> PlipIo::ReadFile(const std::string &path, uintmax_t size) {
        auto file = LoadFile(path);
        return ReadFile(file, size);
    }
}
