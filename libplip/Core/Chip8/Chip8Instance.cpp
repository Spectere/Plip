/* Chip8Instance.cpp
 *
 * A CHIP-8 implementation for Plip.
 *
 * (hahaha, more like PLIP-8 amirite)
 */

#include "Chip8Instance.h"
#include "../../PlipIo.h"

namespace Plip::Core::Chip8 {
    Chip8Instance::Chip8Instance(PlipAudio *audio, PlipInput *input, PlipVideo *video)
    : Plip::PlipCore(audio, input, video) {
        m_ram = new PlipMemoryRam(RamSize);
        m_memoryMap->AddBlock(m_ram);

        m_input->AddInput(0x0, PlipInputDefinition(PlipInputType::Digital, "0"), { .digital = false });
        m_input->AddInput(0x1, PlipInputDefinition(PlipInputType::Digital, "1"), { .digital = false });
        m_input->AddInput(0x2, PlipInputDefinition(PlipInputType::Digital, "2"), { .digital = false });
        m_input->AddInput(0x3, PlipInputDefinition(PlipInputType::Digital, "3"), { .digital = false });
        m_input->AddInput(0x4, PlipInputDefinition(PlipInputType::Digital, "4"), { .digital = false });
        m_input->AddInput(0x5, PlipInputDefinition(PlipInputType::Digital, "5"), { .digital = false });
        m_input->AddInput(0x6, PlipInputDefinition(PlipInputType::Digital, "6"), { .digital = false });
        m_input->AddInput(0x7, PlipInputDefinition(PlipInputType::Digital, "7"), { .digital = false });
        m_input->AddInput(0x8, PlipInputDefinition(PlipInputType::Digital, "8"), { .digital = false });
        m_input->AddInput(0x9, PlipInputDefinition(PlipInputType::Digital, "9"), { .digital = false });
        m_input->AddInput(0xA, PlipInputDefinition(PlipInputType::Digital, "A"), { .digital = false });
        m_input->AddInput(0xB, PlipInputDefinition(PlipInputType::Digital, "B"), { .digital = false });
        m_input->AddInput(0xC, PlipInputDefinition(PlipInputType::Digital, "C"), { .digital = false });
        m_input->AddInput(0xD, PlipInputDefinition(PlipInputType::Digital, "D"), { .digital = false });
        m_input->AddInput(0xE, PlipInputDefinition(PlipInputType::Digital, "E"), { .digital = false });
        m_input->AddInput(0xF, PlipInputDefinition(PlipInputType::Digital, "F"), { .digital = false });
    }

    void Chip8Instance::Delta(long ns) {
    }

    PlipError Chip8Instance::Load(const std::string &path) {
        using io = Plip::PlipIo;
        if(!io::FileExists(path)) return PlipError::FileNotFound;

        auto size = io::GetSize(path);
        auto data = io::ReadFile(path, size);

        // Zero RAM.
        for(auto i = 0; i < RamSize; i++)
            m_memoryMap->SetByte(i, 0x00);

        // Load program.
        auto ramByte = ProgramOffset;
        auto dataByte = 0;
        while(dataByte < size && ramByte < RamSize)
            m_memoryMap->SetByte(ramByte++, data[dataByte++]);

        // Write character set.
        WriteCharacterSet(CharacterSet);

        return PlipError::Success;
    }

    void Chip8Instance::WriteCharacterSet(uint32_t address) {
        for(auto i = 0; i < m_charsetLength; i++)
            m_memoryMap->SetByte(address + i, m_charset[i]);
    }
}
