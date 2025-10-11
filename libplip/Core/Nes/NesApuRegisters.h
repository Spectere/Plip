/* NesApuRegisters.h
 *
 * Implements the NES's APU registers.
 */

#pragma once

#include "../../Memory/PlipMemory.h"

namespace Plip::Core::Nes {
    enum class ApuRegister {
        Pulse1DutyCycleVolume = 0x00,
        Pulse1SweepControl = 0x01,
        Pulse1PeriodLow = 0x02,
        Pulse1PeriodHighLengthControl = 0x03,
        Pulse2DutyCycleVolume = 0x04,
        Pulse2SweepControl = 0x05,
        Pulse2PeriodLow = 0x06,
        Pulse2PeriodHighLengthControl = 0x07,
        TriangleLinearCounter = 0x08,
        TriangleUnused = 0x09,
        TrianglePeriodLow = 0x0A,
        TriangleHighPeriodLengthControl = 0x0B,
        NoiseVolume = 0x0C,
        NoiseUnused = 0x0D,
        NoisePeriodWaveformShape = 0x0E,
        NoiseLengthControl = 0x0F,
        DmcIrqLoopFrequency = 0x10,
        Dmc7BitDac = 0x11,
        DmcStartAddress = 0x12,
        DmcSampleLength = 0x13,
        OamDma = 0x14,
        SoundChannelsEnable = 0x15,
        Joy1 = 0x16,
        Joy2 = 0x17,
    };
    
    class NesApuRegisters final : public PlipMemory {
    public:
        [[nodiscard]] uint8_t GetByte(const uint32_t address, const bool privileged) override {
            return GetByte(static_cast<ApuRegister>(address), privileged);
        }
        
        void SetByte(const uint32_t address, const uint8_t value, const bool privileged) override {
            SetByte(static_cast<ApuRegister>(address), value, privileged);
        }

        [[nodiscard]] uint8_t GetByte(ApuRegister apuRegister, bool privileged);
        [[nodiscard]] uint32_t GetLength() override;
        void SetByte(ApuRegister apuRegister, uint8_t value, bool privileged);

        [[nodiscard]] bool GetControllerStrobe() const { return m_controllerStrobe; }
        void SetControllerPort1(const uint8_t value) { m_joy1 = value; }
        void SetControllerPort2(const uint8_t value) { m_joy2 = value; }
        
    private:
        uint8_t m_pulse1DutyCycleVolume {};
        uint8_t m_pulse1SweepControl {};
        uint8_t m_pulse1PeriodLow {};
        uint8_t m_pulse1PeriodHighLengthControl {};
        uint8_t m_pulse2DutyCycleVolume {};
        uint8_t m_pulse2SweepControl {};
        uint8_t m_pulse2PeriodLow {};
        uint8_t m_pulse2PeriodHighLengthControl {};
        uint8_t m_triangleLinearCounter {};
        uint8_t m_trianglePeriodLow {};
        uint8_t m_trianglePeriodHighLengthControl {};
        uint8_t m_noiseVolume {};
        uint8_t m_noisePeriodWaveformShape {};
        uint8_t m_noiseLengthControl {};
        uint8_t m_dmcIrqLoopFrequency {};
        uint8_t m_dmc7BitDac {};
        uint8_t m_dmcStartAddress {};
        uint8_t m_dmcSampleLength {};
        uint8_t m_oamDma {};
        uint8_t m_soundChannelsEnable {};
        uint8_t m_joy1 {};
        uint8_t m_joy2 {};

        bool m_controllerStrobe {};
    }; 
}
