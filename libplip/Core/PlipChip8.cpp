/* PlipChip8.cpp
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#include "PlipChip8.h"
#include "../PlipIo.h"

namespace Plip::Core {
    PlipChip8::PlipChip8(PlipInput *input) : Plip::PlipCore(input) {
    }

    PlipError PlipChip8::Load(const std::string &path) {
        using io = Plip::PlipIo;
        if(!io::FileExists(path)) return PlipError::FileNotFound;

        auto size = io::GetSize(path);
        auto ram = io::ReadRam(path, io::GetSize(path), 0x200, 0x1000);
        m_memoryMap->AddBlock(&ram);
        return PlipError::Success;
    }
}
