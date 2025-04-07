/* GameBoyIoRegisters.h
 *
 * Implements the Game Boy's I/O registers.
 */

#pragma once

#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemory.h"

namespace Plip::Core::GameBoy {
    enum class IoRegister {
        JoypadInput = 0x00,
        SerialData = 0x01,
        SerialControl = 0x02,
        Divider = 0x04,
        TimerCounter = 0x05,
        TimerModulo = 0x06,
        TimerControl = 0x07,
        InterruptFlag = 0x0F,
        SoundCh1Sweep = 0x10,
        SoundCh1LengthDuty = 0x11,
        SoundCh1VolumeEnvelope = 0x12,
        SoundCh1PeriodLow = 0x13,
        SoundCh1PeriodHighControl = 0x14,
        SoundCh2LengthDuty = 0x16,
        SoundCh2VolumeEnvelope = 0x17,
        SoundCh2PeriodLow = 0x18,
        SoundCh2PeriodHighControl = 0x19,
        SoundCh3DacEnable = 0x1A,
        SoundCh3Length = 0x1B,
        SoundCh3OutputLevel = 0x1C,
        SoundCh3PeriodLow = 0x1D,
        SoundCh3PeriodHighControl = 0x1E,
        SoundCh4Length = 0x20,
        SoundCh4VolumeEnvelope = 0x21,
        SoundCh4FrequencyRandomness = 0x22,
        SoundCh4Control = 0x23,
        SoundVolume = 0x24,
        SoundPanning = 0x25,
        SoundEnable = 0x26,
        WaveRam0 = 0x30,
        WaveRam1 = 0x31,
        WaveRam2 = 0x32,
        WaveRam3 = 0x33,
        WaveRam4 = 0x34,
        WaveRam5 = 0x35,
        WaveRam6 = 0x36,
        WaveRam7 = 0x37,
        WaveRam8 = 0x38,
        WaveRam9 = 0x39,
        WaveRamA = 0x3A,
        WaveRamB = 0x3B,
        WaveRamC = 0x3C,
        WaveRamD = 0x3D,
        WaveRamE = 0x3E,
        WaveRamF = 0x3F,
        LcdControl = 0x40,
        LcdStatus = 0x41,
        ScrollY = 0x42,
        ScrollX = 0x43,
        LcdYCoordinate = 0x44,
        LcdYCompare = 0x45,
        OamDmaSourceAddress = 0x46,
        BgPalette = 0x47,
        Obj0Palette = 0x48,
        Obj1Palette = 0x49,
        WindowY = 0x4A,
        WindowX = 0x4B,
        BootRomDisable = 0x50,
    };
    
    class GameBoyIoRegisters : public PlipMemory {
    public:
        GameBoyIoRegisters();
        ~GameBoyIoRegisters() = default;

        uint8_t GetByte(const uint32_t address, const bool privileged = false) override {
            return GetByte(static_cast<IoRegister>(address));
        }

        void SetByte(const uint32_t address, const uint8_t value, const bool privileged = false) override {
            SetByte(static_cast<IoRegister>(address), value);
        }

        uint8_t GetByte(IoRegister ioRegister) const;
        uint32_t GetLength() override { return Length; }
        void Reset();
        void SetByte(IoRegister ioRegister, uint8_t value);

        bool GetBootRomDisabled() const { return m_bootRomDisabled; }
        void RaiseInterrupt(Cpu::SharpLr35902Interrupt interrupt);

        // Joypad
        void Joypad_Cycle();
        void Joypad_SetMatrix(const uint8_t inputsPressed) { m_inputsPressed = inputsPressed; }

        // Timer
        void Timer_Cycle();
        void Timer_FallingEdgeDetection(bool thisBit);
        static int Timer_GetFrequencyBit(int clockSelect);

        // Video
        void Video_SetYCoordinate(const uint8_t value) { m_regLcdYCoordinate = value; }

    private:
        constexpr static uint32_t Length = 0x80;

        static uint8_t PadValue(const uint8_t value, const uint8_t bits) { return (0b11111111 << bits) | value; }  // Pull unused bits high.

        /*
         * Internal Data
         */
        bool m_bootRomDisabled {};
        uint8_t m_interruptFlag {};

        // Joypad
        uint8_t m_inputsPressed {};
        uint8_t m_inputsSelected {};

        // Timer
        enum TimaReloadStatus {
            NoReload,
            ReloadScheduled,
            ReloadJustOccurred,
        };
        
        bool m_timerDividerReset {};
        bool m_timerLastBitResult {};
        TimaReloadStatus m_timerTimaReloadStatus {};
        uint16_t m_timerRegister {};

        /*
         * Register Values (if they aren't derived from the above)
         */

        // Joypad
        /* $FF00 */ uint8_t m_regJoypad {};

        // Timer
        /* $FF05 */ uint8_t m_regTimerCounter {};
        /* $FF06 */ uint8_t m_regTimerModulo {};
        /* $FF07 */ uint8_t m_regTimerControl {};

        // Video
        /* $FF40 */ uint8_t m_regLcdControl {};
        /* $FF41 */ uint8_t m_regLcdStatus {};
        /* $FF42 */ uint8_t m_regScrollY {};
        /* $FF43 */ uint8_t m_regScrollX {};
        /* $FF44 */ uint8_t m_regLcdYCoordinate {};
        /* $FF45 */ uint8_t m_regLcdYCompare {};
        /* $FF47 */ uint8_t m_regBgPalette {};
        /* $FF4A */ uint8_t m_regWindowY {};
        /* $FF4B */ uint8_t m_regWindowX {};
    };
}
