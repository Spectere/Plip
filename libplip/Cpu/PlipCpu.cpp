/* PlipCpu.cpp
 *
 * Defines a CPU implementation.
 */

#include "PlipCpu.h"

using Plip::Cpu::PlipCpu;

PlipCpu::PlipCpu(const long hz, PlipMemoryMap* memoryMap) {
    SetHz(hz);
    m_memory = memoryMap;
}

long PlipCpu::GetCycleTime() const {
    return m_cycle;
}

long PlipCpu::GetHz() const {
    return m_hz;
}

void PlipCpu::SetHz(const long hz) {
    m_hz = hz;
    m_cycle = 1000000000 / hz;
}
