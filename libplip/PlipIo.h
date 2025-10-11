/* PlipIo.h
 *
 * Standardized functions for dealing with file I/O.
 */

#pragma once

#include <fstream>
#include <vector>

#include "Memory/PlipMemory.h"

namespace Plip {
    class PlipIo {
    public:
        static void CloseFile(std::fstream& file);
        static std::fstream CreateFile(const std::string& path);
        static void DumpMemoryToDisk(std::fstream& file, PlipMemory *memory);
        static void DumpMemoryToDisk(const std::string& path, PlipMemory *memory);
        static bool FileExists(const std::string& path);
        static std::string GetFilename(const std::string& path);
        static uintmax_t GetSize(const std::string& path);
        static std::fstream LoadFile(const std::string& path, bool writable = false);
        static std::vector<char> ReadFile(std::fstream& file, uintmax_t size);
        static std::vector<char> ReadFile(const std::string& path, uintmax_t size);
        static std::vector<char> ReadFile(std::fstream& file, int offset, uintmax_t size);
        static std::vector<char> ReadFile(const std::string& path, int offset, uintmax_t size);
        static std::vector<char> ReadSequential(std::fstream& file, uintmax_t size);

    private:
        PlipIo() = default;
    };
}
