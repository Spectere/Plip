// RunnerCpu.h
//
// A CPU that can accept Jsmoo unit tests.

#pragma once

#include "RunnerCpuError.h"
#include "RunnerTestState.h"
#include "Memory/PlipMemory.h"
#include "Memory/PlipMemoryMap.h"
#include "Memory/PlipMemoryRam.h"

class RunnerCpu {
public:
    explicit RunnerCpu(const uint32_t memorySize) {
        CreateMemoryMap(memorySize);
    }

    [[nodiscard]] uint8_t ReadMemory(const uint32_t position) const { return memory->GetByte(position); }
    void WriteMemory(const uint32_t position, const uint8_t value) const { memory->SetByte(position, value); }

    virtual std::vector<RunnerCpuError> CompareState(RunnerTestState state) = 0;
    virtual bool CycleAccurateMemoryTest() = 0;
    virtual uint32_t GetPc() = 0;
    virtual void SetInitialState(RunnerTestState state) = 0;

    virtual MemoryBusState GetMemoryBusState() const = 0;
    virtual uint64_t GetAddressBus() const = 0;
    virtual uint8_t GetDataBus() const = 0;

    virtual void Cycle() = 0;

    Plip::PlipMemoryMap* memory {};

protected:
    ~RunnerCpu() {
        delete memory;
        delete m_ram;
    }

private:
    Plip::PlipMemoryRam* m_ram {};

    void CreateMemoryMap(const uint32_t size) {
        memory = new Plip::PlipMemoryMap();
        m_ram = new Plip::PlipMemoryRam(size);
        memory->AddBlock(m_ram, 0);
    }
};
