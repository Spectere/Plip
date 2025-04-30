/* MockCpu.h
 *
 * A class that can be easily used to create new mock CPUs.
 */

#pragma once

#include <vector>

#include "Cpu/PlipCpu.h"
#include "Memory/PlipMemoryMap.h"
#include "Memory/PlipMemoryRam.h"

#define CHECK_RAM(addr, val) CHECK(memory->GetByte(addr) == val);

template<class CpuType>
class MockCpu {
    static_assert(std::is_base_of_v<Plip::Cpu::PlipCpu, CpuType>, "CpuType must be a subclass of PlipCpu");

public:
    MockCpu() : MockCpu(0) {}

    explicit MockCpu(uint32_t pc) {
        memory = CreateMockMemoryMap();

        cpu = CreateMockCpu(memory);
        cpu->Reset(pc);
    }

    void LoadData(int offset, const std::vector<uint8_t>& data) const {
        for(const auto byte : data) {
            memory->SetByte(offset++, byte);
        }
    }

    CpuType* cpu;
    Plip::PlipMemoryMap* memory;

private:
    static Plip::PlipMemoryMap* CreateMockMemoryMap() {
        const auto memoryMap = new Plip::PlipMemoryMap();
        memoryMap->AddBlock(new Plip::PlipMemoryRam(0x10000), 0);
        
        return memoryMap;
    }

    static CpuType* CreateMockCpu(Plip::PlipMemoryMap* memoryMap) {
        return new CpuType(1000, memoryMap);
    }
};
