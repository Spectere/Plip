/* GameBoyIoRegisters.h
 *
 * Implements the Game Boy's I/O registers.
 */

#pragma once

#include "GameBoyModel.h"
#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemory.h"

namespace Plip::Core::GameBoy {
    enum class DmaTransferMode {
        Inactive,
        Oam,
        GeneralPurpose,
        HBlank,
    };
    
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
        CpuModeSelect = 0x4C,           // CGB
        SpeedSwitch = 0x4D,             // CGB
        VramBank = 0x4F,                // CGB
        BootRomDisable = 0x50,
        VramDmaSourceHigh = 0x51,       // CGB
        VramDmaSourceLow = 0x52,        // CGB
        VramDmaDestinationHigh = 0x53,  // CGB
        VramDmaDestinationLow = 0x54,   // CGB
        VramDmaLengthModeStart = 0x55,  // CGB
        InfraredPort = 0x56,            // CGB
        BackgroundPaletteIndex = 0x68,  // CGB
        BackgroundPaletteData = 0x69,   // CGB
        ObjectPaletteIndex = 0x6A,      // CGB
        ObjectPaletteData = 0x6B,       // CGB
        ObjectPriorityMode = 0x6C,      // CGB
        WramBank = 0x70,                // CGB
        AudioDigitalOutput12 = 0x76,    // CGB
        AudioDigitalOutput34 = 0x77,    // CGB
    };
    
    class GameBoyIoRegisters final : public PlipMemory {
    public:
        GameBoyIoRegisters(GameBoyModel gbModel, PlipMemory* cgbBgPaletteRam, PlipMemory* cgbObjPaletteRam);
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

        // Core
        void AcknowledgeWorkRamBankSwitch() { m_performWorkRamBankSwitch = -1; }
        bool GetBootRomDisabled() const { return m_bootRomDisabled; }
        bool GetIsDoubleSpeedArmed() const { return m_speedSwitchArmed; }
        int GetPerformWorkRamBankSwitch() const { return m_performWorkRamBankSwitch; }
        void RaiseInterrupt(Cpu::SharpLr35902Interrupt interrupt);
        void SetDoubleSpeed(const bool doubleSpeed) {
            m_doubleSpeedActive = doubleSpeed;
            m_speedSwitchArmed = false;
        }

        // Joypad
        void Joypad_Cycle();
        void Joypad_SetMatrix(const uint8_t inputsPressed) { m_inputsPressed = inputsPressed; }

        // Timer
        void Timer_Cycle();
        void Timer_FallingEdgeDetection(bool thisBit);
        static int Timer_GetFrequencyBit(int clockSelect);
        void Timer_Reset() { m_timerInternal = 0; }

        // Audio
        bool Audio_GetEnabled() const { return m_audioEnabled; }
        uint8_t Audio_GetChannelPanning() const { return m_audioChannelPanning; }
        uint8_t Audio_GetMasterVolume() const { return m_audioMasterVolume; }
        uint8_t Audio_GetVinPanning() const { return m_audioVinPanning; }
        void Audio_SetChannelState(const uint8_t state) { m_audioChannelState = state & 0xF; }

        // Video
        void Video_AcknowledgeOamDmaCopy() { m_videoPerformOamDmaCopy = false; }
        void Video_AcknowledgeHdmaCancellation() { m_videoHdmaTransferCancelled = false; }
        void Video_AcknowledgeVideoRamBankSwitch() { m_videoPerformVideoRamBankSwitch = -1; }
        int Video_GetOamDmaCopyAddress() const { return m_videoPerformOamDmaCopy ? m_regOamDmaAddress : -1; }
        int Video_GetHdmaDestinationAddress() const { return m_videoHdmaDestinationAddress; }
        int Video_GetHdmaSourceAddress() const { return m_videoHdmaSourceAddress; }
        bool Video_GetHdmaTransferCancelled() const { return m_videoHdmaTransferCancelled; }
        int Video_GetHdmaTransferLength() const { return m_videoHdmaTransferLength; }
        DmaTransferMode Video_GetHdmaTransferMode() const { return m_videoHdmaTransferMode; }
        int Video_GetPerformVramBankSwitch() const { return m_videoPerformVideoRamBankSwitch; }
        void Video_SetHdmaTransferComplete() {
            m_videoHdmaTransferMode = DmaTransferMode::Inactive;
            m_videoHdmaTransferRemaining = 0xFF;
        }
        void Video_SetHdmaTransferRemaining(const int remaining) { m_videoHdmaTransferRemaining = remaining; }
        void Video_SetLcdStatus(const uint8_t value) { m_regLcdStatus = (m_regLcdStatus & 0b11111000) | (value & 0b111); }
        void Video_SetYCoordinate(const uint8_t value) { m_regLcdYCoordinate = value; }

    private:
        constexpr static uint32_t Length = 0x80;

        static uint8_t PadValue(const uint8_t value, const uint8_t usedBits) { return (0b11111111 << usedBits) | value; }  // Pull unused bits high.

        /*
         * Internal Data
         */

        // Core
        bool m_bootRomDisabled {};
        uint8_t m_interruptFlag {};
        GameBoyModel m_model {};
        bool m_dmgCompatibility = true;                 // CGB
        int m_performWorkRamBankSwitch = -1;            // CGB
        bool m_doubleSpeedActive = false;               // CGB
        bool m_speedSwitchArmed = false;                // CGB

        // Joypad
        uint8_t m_inputsPressed {};
        uint8_t m_inputsSelected {};

        // Timer
        enum TimaReloadStatus {
            NoReload,
            ReloadScheduled,
            ReloadJustOccurred,
        };
        
        bool m_timerLastBitResult {};
        TimaReloadStatus m_timerTimaReloadStatus {};
        uint16_t m_timerInternal {};

        // Audio
        bool m_audioEnabled {};
        uint8_t m_audioChannelPanning {};
        uint8_t m_audioChannelState {};
        uint8_t m_audioMasterVolume {};
        uint8_t m_audioVinPanning {};
        uint8_t m_audioWaveRam[16] {};

        // Video
        bool m_videoPerformOamDmaCopy {};
        PlipMemory* m_videoCgbBgPaletteRam {};          // CGB
        PlipMemory* m_videoCgbObjPaletteRam {};         // CGB
        bool m_videoPerformHdmaCopy {};                 // CGB
        int m_videoPerformVideoRamBankSwitch = -1;      // CGB
        uint16_t m_videoHdmaDestinationAddress {};      // CGB
        uint16_t m_videoHdmaSourceAddress {};           // CGB
        bool m_videoHdmaTransferCancelled {};           // CGB
        int m_videoHdmaTransferLength {};               // CGB
        DmaTransferMode m_videoHdmaTransferMode {};    // CGB
        int m_videoHdmaTransferRemaining {};            // CGB
        bool m_videoCgbObjectPriority {};               // CGB
        bool m_videoBgPaletteAutoIncrement {};          // CGB
        int m_videoBgPaletteIndex {};                   // CGB
        bool m_videoObjPaletteAutoIncrement {};         // CGB
        int m_videoObjPaletteIndex {};                  // CGB

        /*
         * Register Values (if they aren't derived from the above)
         */

        // Core
        /* $FF70 */ uint8_t m_regWramBank {};           // CGB

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
        /* $FF46 */ uint8_t m_regOamDmaAddress {};
        /* $FF47 */ uint8_t m_regDmgBgPalette {};
        /* $FF48 */ uint8_t m_regDmgObj0Palette {};
        /* $FF49 */ uint8_t m_regDmgObj1Palette {};
        /* $FF4A */ uint8_t m_regWindowY {};
        /* $FF4B */ uint8_t m_regWindowX {};
        /* $FF4F */ uint8_t m_regVramBank {};           // CGB
    };
}
