/* NesApuRegisters.cpp
 *
 * Implements the NES's APU registers.
 */

#include "NesApuRegisters.h"

using Plip::Core::Nes::NesApuRegisters;

uint8_t NesApuRegisters::GetByte(const ApuRegister apuRegister, [[maybe_unused]] bool privileged) {
    constexpr uint8_t openBus = 0xFF;  // TODO: I don't think this is accurate on the NES. Correct this later.
    
    switch(apuRegister) {
        case ApuRegister::Pulse1DutyCycleVolume:
        case ApuRegister::Pulse1SweepControl:
        case ApuRegister::Pulse1PeriodLow:
        case ApuRegister::Pulse1PeriodHighLengthControl:
        case ApuRegister::Pulse2DutyCycleVolume:
        case ApuRegister::Pulse2SweepControl:
        case ApuRegister::Pulse2PeriodLow:
        case ApuRegister::Pulse2PeriodHighLengthControl:
        case ApuRegister::TriangleLinearCounter:
        case ApuRegister::TriangleUnused:
        case ApuRegister::TrianglePeriodLow:
        case ApuRegister::TriangleHighPeriodLengthControl:
        case ApuRegister::NoiseVolume:
        case ApuRegister::NoiseUnused:
        case ApuRegister::NoisePeriodWaveformShape:
        case ApuRegister::NoiseLengthControl:
        case ApuRegister::DmcIrqLoopFrequency:
        case ApuRegister::Dmc7BitDac:
        case ApuRegister::DmcStartAddress:
        case ApuRegister::DmcSampleLength:
        case ApuRegister::OamDma:
        default: {
            return openBus;
        }
        case ApuRegister::SoundChannelsEnable: { return m_soundChannelsEnable; }
        case ApuRegister::Joy1: {
            const uint8_t value = m_joy1 & 1;
            m_joy1 >>= 1;
            return value;
        }
        case ApuRegister::Joy2: {
            const uint8_t value = m_joy2 & 1;
            m_joy2 >>= 1;
            return value;
        }
    }
}

uint32_t NesApuRegisters::GetLength() {
    return 0x20;
}

void NesApuRegisters::SetByte(const ApuRegister apuRegister, const uint8_t value, [[maybe_unused]] bool privileged) {
    switch(apuRegister) {
        case ApuRegister::Pulse1DutyCycleVolume: {
            m_pulse1DutyCycleVolume = value;
            break;
        }
        case ApuRegister::Pulse1SweepControl: {
            m_pulse1SweepControl = value;
            break;
        }
        case ApuRegister::Pulse1PeriodLow: {
            m_pulse1PeriodLow = value;
            break;
        }
        case ApuRegister::Pulse1PeriodHighLengthControl: {
            m_pulse1PeriodHighLengthControl = value;
            break;
        }
        case ApuRegister::Pulse2DutyCycleVolume: {
            m_pulse2DutyCycleVolume = value;
            break;
        }
        case ApuRegister::Pulse2SweepControl: {
            m_pulse2SweepControl = value;
            break;
        }
        case ApuRegister::Pulse2PeriodLow: {
            m_pulse2PeriodLow = value;
            break;
        }
        case ApuRegister::Pulse2PeriodHighLengthControl: {
            m_pulse2PeriodHighLengthControl = value;
            break;
        }
        case ApuRegister::TriangleLinearCounter: {
            m_triangleLinearCounter = value;
            break;
        }
        case ApuRegister::TrianglePeriodLow: {
            m_trianglePeriodLow = value;
            break;
        }
        case ApuRegister::TriangleHighPeriodLengthControl: {
            m_trianglePeriodHighLengthControl = value;
            break;
        }
        case ApuRegister::NoiseVolume: {
            m_noiseVolume = value;
            break;
        }
        case ApuRegister::NoisePeriodWaveformShape: {
            m_noisePeriodWaveformShape = value;
            break;
        }
        case ApuRegister::NoiseLengthControl: {
            m_noiseLengthControl = value;
            break;
        }
        case ApuRegister::DmcIrqLoopFrequency: {
            m_dmcIrqLoopFrequency = value;
            break;
        }
        case ApuRegister::Dmc7BitDac: {
            m_dmc7BitDac = value;
            break;
        }
        case ApuRegister::DmcStartAddress: {
            m_dmcStartAddress = value;
            break;
        }
        case ApuRegister::DmcSampleLength: {
            m_dmcSampleLength = value;
            break;
        }
        case ApuRegister::OamDma: {
            m_oamDma = value;
            break;
        }
        case ApuRegister::SoundChannelsEnable: {
            m_soundChannelsEnable = value;
            break;
        }
        case ApuRegister::Joy1: {
            m_controllerStrobe = value & 1;
            break;
        }
        case ApuRegister::Joy2: {
            m_joy2 = value;
            break;
        }
        case ApuRegister::TriangleUnused:
        case ApuRegister::NoiseUnused: {
            break;
        }
    }
}
