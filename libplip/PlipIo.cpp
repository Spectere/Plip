/* PlipIo.cpp
 *
 * Standardized functions for dealing with file I/O.
 */

#include <filesystem>

#include "PlipIo.h"

using Plip::PlipIo;

bool PlipIo::FileExists(const std::string &path) {
    std::ifstream file(path.c_str());
    const bool success = file.good();
    if(success) file.close();
    return success;
}

uintmax_t PlipIo::GetSize(const std::string &path) {
    namespace fs = std::filesystem;
    return file_size(fs::path(path));
}

std::ifstream PlipIo::LoadFile(const std::string &path) {
    return std::ifstream(path.c_str());
}

std::vector<char> PlipIo::ReadFile(std::ifstream &file, const uintmax_t size) {
    std::vector<char> data(size);

    // Unlikely, but let's handle it anyway.
    if(size > std::numeric_limits<std::streamsize>::max()) {
        throw std::runtime_error("File size is larger than supported.");
    }

    file.seekg(std::istream::beg);
    file.read(data.data(), static_cast<long>(data.size()));
    return data;
}

std::vector<char> PlipIo::ReadFile(const std::string &path, const uintmax_t size) {
    auto file = LoadFile(path);
    return ReadFile(file, size);
}
