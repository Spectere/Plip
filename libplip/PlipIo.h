/* PlipIo.h
 *
 * Standardized functions for dealing with file I/O.
 */

#pragma once

#include <fstream>
#include <istream>
#include <vector>

#include "Input/PlipInput.h"
#include "Memory/PlipMemoryRam.h"
#include "Memory/PlipMemoryRom.h"

namespace Plip {
    class PlipIo {
    public:
        static bool FileExists(const std::string &path);
        static uintmax_t GetSize(const std::string &path);
        static std::ifstream LoadFile(const std::string &path);
        static std::vector<char> ReadFile(std::ifstream &file, uintmax_t size);
        static std::vector<char> ReadFile(const std::string &path, uintmax_t size);

    private:
        PlipIo() = default;
    };
}
