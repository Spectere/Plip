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
        memory = CreateMemoryMap(memorySize);
    }

    [[nodiscard]] uint8_t ReadMemory(const uint32_t position) const { return memory->GetByte(position); }
    void WriteMemory(const uint32_t position, const uint8_t value) const { memory->SetByte(position, value); }

    virtual std::vector<RunnerCpuError> CompareState(RunnerTestState state) = 0;
    virtual void SetInitialState(RunnerTestState state) = 0;

    virtual void Step() = 0;

    Plip::PlipMemoryMap* memory;

protected:
    ~RunnerCpu() = default;

private:
    static Plip::PlipMemoryMap* CreateMemoryMap(const uint32_t size) {
        const auto memoryMap = new Plip::PlipMemoryMap();
        memoryMap->AddBlock(new Plip::PlipMemoryRam(size), 0);

        return memoryMap;
    }
};
