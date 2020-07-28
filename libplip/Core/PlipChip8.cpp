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
        m_ram = new PlipMemoryRam(RamSize);
    }

    PlipError PlipChip8::Load(const std::string &path) {
        using io = Plip::PlipIo;
        if(!io::FileExists(path)) return PlipError::FileNotFound;

        auto size = io::GetSize(path);
        auto data = io::ReadFile(path, size);

        // Zero RAM.
        for(auto i = 0; i < RamSize; i++)
            m_ram->SetByte(i, 0x00);

        // Load program.
        auto ramByte = ProgramOffset;
        auto dataByte = 0;
        while(dataByte < size && ramByte < RamSize)
            m_ram->SetByte(ramByte++, data[dataByte++]);

        m_memoryMap->AddBlock(m_ram);
        return PlipError::Success;
    }
}
