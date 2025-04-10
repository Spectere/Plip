/* PlipIo.cpp
 *
 * Standardized functions for dealing with file I/O.
 */

#include <filesystem>

#include "PlipIo.h"

using Plip::PlipIo;

std::fstream PlipIo::CreateFile(const std::string &path) {
    return std::fstream(path.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
}

void PlipIo::DumpMemoryToDisk(std::fstream &file, PlipMemory *memory) {
    const auto memoryLength = memory->GetLength();
    
    for(auto i = 0; i < memoryLength; i++) {
        file.put(memory->GetByte(i, true));
    }
    file.flush();
}

void PlipIo::DumpMemoryToDisk(const std::string &path, PlipMemory *memory) {
    auto file = CreateFile(path);
    DumpMemoryToDisk(file, memory);
    file.close();
}

bool PlipIo::FileExists(const std::string &path) {
    std::ifstream file(path.c_str());
    const bool success = file.good();
    if(success) file.close();
    return success;
}

std::string PlipIo::GetFilename(const std::string &path) {
    const auto fsPath = std::filesystem::path(path);
    return fsPath.filename().string();
}


uintmax_t PlipIo::GetSize(const std::string &path) {
    namespace fs = std::filesystem;
    return file_size(fs::path(path));
}

std::fstream PlipIo::LoadFile(const std::string &path, bool writable) {
    return std::fstream(
        path.c_str(),
        writable
            ? std::ios_base::in | std::ios_base::out | std::ios_base::binary
            : std::ios_base::in | std::ios_base::binary
    );
}

std::vector<char> PlipIo::ReadFile(std::fstream &file, const uintmax_t size) {
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
    const auto contents = ReadFile(file, size);
    file.close();
    return contents;
}
