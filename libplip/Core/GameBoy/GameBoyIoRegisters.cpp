/* GameBoyIoRegisters.cpp
 *
 * Implements the Game Boy's I/O registers.
 */

#include "GameBoyIoRegisters.h"
#include "../../PlipSupport.h"

using Plip::Core::GameBoy::AudioNoiseDefinition;
using Plip::Core::GameBoy::AudioPulseDefinition;
using Plip::Core::GameBoy::AudioWaveDefinition;
using Plip::Core::GameBoy::GameBoyIoRegisters;

#define CGB_SUPPORT (m_model == GameBoyModel::CGB)

GameBoyIoRegisters::GameBoyIoRegisters(const GameBoyModel gbModel, PlipMemory* cgbBgPaletteRam, PlipMemory* cgbObjPaletteRam)
                                     : m_model(gbModel), m_videoCgbBgPaletteRam(cgbBgPaletteRam), m_videoCgbObjPaletteRam(cgbObjPaletteRam) {
    Reset();
}

void GameBoyIoRegisters::Audio_DivApuIncrementCheck() {
    const int bitCheck = m_doubleSpeedActive ? 13 : 12;
    const bool thisBit = m_timerInternal & (0b1 << bitCheck);

    if((m_audioDivApuIncremented = !thisBit && m_audioDivApuLastBit)) {
        ++m_audioDivApu;
    }
    m_audioDivApuLastBit = thisBit;
}

AudioPulseDefinition GameBoyIoRegisters::Audio_GetCh1Data() {
    m_audioCh1Triggered = false;
    return {
        .PeriodSweepPace = (m_audioCh1Sweep & 0b01110000) >> 4,
        .PeriodSweepStep = (m_audioCh1Sweep & 0b00000111) * ((m_audioCh1Sweep & 0b00001000) ? -1 : 1),
        .DutyCycle = m_audioCh1LengthAndDutyCycle >> 6,
        .InitialLengthTimer = m_audioCh1LengthAndDutyCycle & 0b00111111,
        .InitialVolume = m_audioCh1VolumeAndEnvelope >> 4,
        .EnvelopeSweepPace = (m_audioCh1VolumeAndEnvelope & 0b00000111) * ((m_audioCh1VolumeAndEnvelope & 0b00001000) ? -1 : 1),
        .Period = ((m_audioCh1PeriodHighAndControl & 0b00000111) << 8) | m_audioCh1PeriodLow,
        .LengthEnable = (m_audioCh1PeriodHighAndControl & 0b01000000) != 0
    };
}

AudioPulseDefinition GameBoyIoRegisters::Audio_GetCh2Data() {
    m_audioCh2Triggered = false;
    return {
        .PeriodSweepPace = 0,
        .PeriodSweepStep = 0,
        .DutyCycle = m_audioCh2LengthAndDutyCycle >> 6,
        .InitialLengthTimer = m_audioCh2LengthAndDutyCycle & 0b00111111,
        .InitialVolume = m_audioCh2VolumeAndEnvelope >> 4,
        .EnvelopeSweepPace = (m_audioCh2VolumeAndEnvelope & 0b00000111) * ((m_audioCh2VolumeAndEnvelope & 0b00001000) ? -1 : 1),
        .Period = ((m_audioCh2PeriodHighAndControl & 0b00000111) << 8) | m_audioCh2PeriodLow,
        .LengthEnable = (m_audioCh2PeriodHighAndControl & 0b01000000) != 0
    };
}

AudioWaveDefinition GameBoyIoRegisters::Audio_GetCh3Data() {
    m_audioCh3Triggered = false;
    return {
        .InitialLengthTimer = m_audioCh3LengthTimer,
        .OutputVolume = (m_audioCh3OutputLevel & 0b01100000) >> 5,
        .Period = ((m_audioCh3PeriodHighAndControl & 0b00000111) << 8) | m_audioCh3PeriodLow,
        .LengthEnable = (m_audioCh3PeriodHighAndControl & 0b01000000) != 0
    };
}

AudioNoiseDefinition GameBoyIoRegisters::Audio_GetCh4Data() {
    m_audioCh4Triggered = false;
    return {
        .InitialLengthTimer = m_audioCh4LengthTimer & 0b00111111,
        .InitialVolume = m_audioCh4VolumeAndEnvelope >> 4,
        .EnvelopeSweepPace = (m_audioCh4VolumeAndEnvelope & 0b00000111) * ((m_audioCh4VolumeAndEnvelope & 0b00001000) ? -1 : 1),
        .ClockShift = m_audioCh4FrequencyAndRandomness >> 4,
        .LfsrWidthWide = (m_audioCh4FrequencyAndRandomness & 0b00001000) != 0,
        .ClockDivider = m_audioCh4FrequencyAndRandomness & 0b00000111,
        .LengthEnable = (m_audioCh4Control & 0b01000000) != 0
    };
}

bool GameBoyIoRegisters::Audio_IsChannelTriggered(const int channel) const {
    switch(channel) {
        case 1: return m_audioCh1Triggered;
        case 2: return m_audioCh2Triggered;
        case 3: return m_audioCh3Triggered;
        case 4: return m_audioCh4Triggered;
        default: return false;
    }
}

void GameBoyIoRegisters::Audio_ResetRegisters() {
    constexpr auto resetValue = 0xFF;

    /*
     * Clear all audio registers (except for wave RAM).
     */

    // Channel 1
    m_audioCh1Sweep = resetValue;
    m_audioCh1LengthAndDutyCycle = resetValue;
    m_audioCh1VolumeAndEnvelope = resetValue;
    m_audioCh1PeriodLow = resetValue;
    m_audioCh1PeriodHighAndControl = resetValue;

    // Channel 2
    m_audioCh2LengthAndDutyCycle = resetValue;
    m_audioCh2VolumeAndEnvelope = resetValue;
    m_audioCh2PeriodLow = resetValue;
    m_audioCh2PeriodHighAndControl = resetValue;

    // Channel 3
    m_audioCh3DacEnable = false;
    m_audioCh3LengthTimer = resetValue;
    m_audioCh3OutputLevel = resetValue;
    m_audioCh3PeriodLow = resetValue;
    m_audioCh3PeriodHighAndControl = resetValue;

    // Channel 4
    m_audioCh4LengthTimer = resetValue;
    m_audioCh4VolumeAndEnvelope = resetValue;
    m_audioCh4FrequencyAndRandomness = resetValue;
    m_audioCh4Control = resetValue;
}

uint8_t GameBoyIoRegisters::GetByte(const IoRegister ioRegister) const {
    if(ioRegister >= IoRegister::WaveRam0 && ioRegister <= IoRegister::WaveRamF) {
        // Wave RAM is basically an array, so treat it as such.
        // TODO: Implement wave RAM (in)accessibility.
        return m_audioWaveRam[static_cast<int>(ioRegister) - static_cast<int>(IoRegister::WaveRam0)];
    }

    switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
        /* $FF00 */ case IoRegister::JoypadInput: { return m_regJoypad; }
        /* $FF04 */ case IoRegister::Divider: { return m_timerInternal >> 8; }
        /* $FF05 */ case IoRegister::TimerCounter: { return m_regTimerCounter; }
        /* $FF06 */ case IoRegister::TimerModulo: { return m_regTimerModulo; }
        /* $FF07 */ case IoRegister::TimerControl: { return m_regTimerControl; }
        /* $FF0F */ case IoRegister::InterruptFlag: { return m_interruptFlag; }
        /* $FF10 */ case IoRegister::SoundCh1Sweep: { return m_audioCh1Sweep; }
        /* $FF11 */ case IoRegister::SoundCh1LengthDuty: { return m_audioCh1LengthAndDutyCycle; }
        /* $FF12 */ case IoRegister::SoundCh1VolumeEnvelope: { return m_audioCh1VolumeAndEnvelope | 0b00111111; }
        /* $FF13 - write-only */
        /* $FF14 */ case IoRegister::SoundCh1PeriodHighControl: { return m_audioCh1PeriodHighAndControl | 0b10000111; }
        /* $FF16 */ case IoRegister::SoundCh2LengthDuty: { return m_audioCh2LengthAndDutyCycle; }
        /* $FF17 */ case IoRegister::SoundCh2VolumeEnvelope: { return m_audioCh2VolumeAndEnvelope | 0b00111111; }
        /* $FF18 */ case IoRegister::SoundCh2PeriodLow: { return m_audioCh2PeriodLow; }
        /* $FF19 */ case IoRegister::SoundCh2PeriodHighControl: { return m_audioCh2PeriodHighAndControl | 0b10000111; }
        /* $FF1A */ case IoRegister::SoundCh3DacEnable: { return m_audioCh3DacEnable ? 0b10000000 : 0; }
        /* $FF1B - write-only */
        /* $FF1C */ case IoRegister::SoundCh3OutputLevel: { return m_audioCh3OutputLevel << 5; }
        /* $FF1D */ case IoRegister::SoundCh3PeriodLow: { return m_audioCh3PeriodLow; }
        /* $FF1E */ case IoRegister::SoundCh3PeriodHighControl: { return m_audioCh3PeriodHighAndControl | 0b10000111; }
        /* $FF20 - write-only */
        /* $FF21 */ case IoRegister::SoundCh4VolumeEnvelope: { return m_audioCh4VolumeAndEnvelope; }
        /* $FF22 */ case IoRegister::SoundCh4FrequencyRandomness: { return m_audioCh4FrequencyAndRandomness; }
        /* $FF23 */ case IoRegister::SoundCh4Control: { return m_audioCh4Control; }
        /* $FF24 */ case IoRegister::SoundVolume: { return m_audioVinPanning | m_audioMasterVolume; }
        /* $FF25 */ case IoRegister::SoundPanning: { return m_audioChannelPanning; }
        /* $FF26 */ case IoRegister::SoundEnable: { return (m_audioEnabled ? 0b10000000 : 0) | m_audioChannelState; }
        /* $FF40 */ case IoRegister::LcdControl: { return m_regLcdControl; }
        /* $FF41 */ case IoRegister::LcdStatus: { return m_regLcdStatus; }
        /* $FF42 */ case IoRegister::ScrollY: { return m_regScrollY; }
        /* $FF43 */ case IoRegister::ScrollX: { return m_regScrollX; }
        /* $FF44 */ case IoRegister::LcdYCoordinate: { return m_regLcdYCoordinate; }
        /* $FF45 */ case IoRegister::LcdYCompare: { return m_regLcdYCompare; }
        /* $FF46 */ case IoRegister::OamDmaSourceAddress: { return m_regOamDmaAddress; }
        /* $FF47 */ case IoRegister::BgPalette: { return m_regDmgBgPalette; }
        /* $FF48 */ case IoRegister::Obj0Palette: { return m_regDmgObj0Palette; }
        /* $FF49 */ case IoRegister::Obj1Palette: { return m_regDmgObj1Palette; }
        /* $FF4A */ case IoRegister::WindowY: { return m_regWindowY; }
        /* $FF4B */ case IoRegister::WindowX: { return m_regWindowX; }
        /* $FF50 */ case IoRegister::BootRomDisable: { return m_bootRomDisabled ? 0b1 : 0b0; }

        default: break;
    }

    if(CGB_SUPPORT) {
        switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
            /* $FF4C */ case IoRegister::CpuModeSelect: { return !m_dmgCompatibility ? 0b11111011 : 0b11111111; }
            /* $FF4D */ case IoRegister::SpeedSwitch: { return 0b01111110 | (m_doubleSpeedActive ? 0b10000000 : 0) | (m_speedSwitchArmed ? 0b1 : 0); }
            /* $FF4F */ case IoRegister::VramBank: { return m_regVramBank; }
            /* $FF55 */ case IoRegister::VramDmaLengthModeStart: {
                return (m_videoHdmaTransferMode != DmaTransferMode::Inactive ? 0b10000000 : 0)
                     | ((m_videoHdmaTransferRemaining >> 4) & 0b01111111);
            }
            /* $FF68 */ case IoRegister::BackgroundPaletteIndex: { return (m_videoBgPaletteAutoIncrement ? 0b10000000 : 0) | 0b01000000 | (m_videoBgPaletteIndex & 0b111111); }
            /* $FF69 */ case IoRegister::BackgroundPaletteData: { return m_videoCgbBgPaletteRam->GetByte(m_videoBgPaletteIndex); }
            /* $FF6A */ case IoRegister::ObjectPaletteIndex: { return (m_videoObjPaletteAutoIncrement ? 0b10000000 : 0) | 0b01000000 | (m_videoObjPaletteIndex & 0b111111); }
            /* $FF6B */ case IoRegister::ObjectPaletteData: { return m_videoCgbObjPaletteRam->GetByte(m_videoObjPaletteIndex); }
            /* $FF6C */ case IoRegister::ObjectPriorityMode: { return 0xFF ^ (m_videoCgbObjectPriority ? 1 : 0); }
            /* $FF70 */ case IoRegister::WramBank: { return m_regWramBank; }
            
            default: break;
        }
    }

    return 0xFF;
}

void GameBoyIoRegisters::Joypad_Cycle() {
    m_regJoypad |= 0b11001111;
    if(!BIT_TEST(m_regJoypad, 5)) {
        // Read buttons.
        m_regJoypad ^= m_inputsPressed & 0b1111;
    } else if(!BIT_TEST(m_regJoypad, 4)) {
        // Read d-pad.
        m_regJoypad ^= m_inputsPressed >> 4;
    }
}

void GameBoyIoRegisters::RaiseInterrupt(const Cpu::SharpLr35902Interrupt interrupt) {
    m_interruptFlag = m_interruptFlag | static_cast<int>(interrupt);
}

void GameBoyIoRegisters::Reset() {
    m_bootRomDisabled = false;
    m_interruptFlag = 0;

    // Timer
    m_timerInternal = 0;
    m_regTimerCounter = 0;
    m_regTimerControl = 0xF8;
    m_regTimerModulo = 0;
}

void GameBoyIoRegisters::SetByte(const IoRegister ioRegister, const uint8_t value) {
    if(ioRegister >= IoRegister::WaveRam0 && ioRegister <= IoRegister::WaveRamF) {
        // Wave RAM is basically an array, so treat it as such.
        // TODO: Implement wave RAM (in)accessibility.
        m_audioWaveRam[static_cast<int>(ioRegister) - static_cast<int>(IoRegister::WaveRam0)] = value;
        return;
    }
    
    switch(ioRegister) {
        // $FF00
        case IoRegister::JoypadInput: {
            m_regJoypad = value;
            break;
        }

        // $FF04
        case IoRegister::Divider: {
            m_timerInternal = 0;
            break;
        }

        // $FF05
        case IoRegister::TimerCounter: {
            switch(m_timerTimaReloadStatus) {
                case NoReload:
                    m_regTimerCounter = value;
                    break;
                case ReloadScheduled:
                    // Abort TMA reload and prevent interrupt from being sent.
                    m_regTimerCounter = value;
                    m_timerTimaReloadStatus = NoReload;
                    break;
                case ReloadJustOccurred:
                    // Cancel attempted write.
                    break;
            }
            break;
        }

        // $FF06
        case IoRegister::TimerModulo: {
            m_regTimerModulo = value;

            if(m_timerTimaReloadStatus == ReloadJustOccurred) {
                // If TMA is written to on the cycle that the reload occurs, the new
                // value will be loaded into TIMA.
                m_regTimerCounter = m_regTimerModulo;
            }
            break;
        }

        // $FF07
        case IoRegister::TimerControl: {
            m_regTimerControl = PadValue(value, 3);
            break;
        }

        // $FF0F
        case IoRegister::InterruptFlag: {
            m_interruptFlag = PadValue(value, 5);
            break;
        }

        // $FF10
        case IoRegister::SoundCh1Sweep: {
            m_audioCh1Sweep = PadValue(value, 6);
            break;
        }

        // $FF11
        case IoRegister::SoundCh1LengthDuty: {
            m_audioCh1LengthAndDutyCycle = value | 0b00111111;
            break;
        }

        // $FF12
        case IoRegister::SoundCh1VolumeEnvelope: {
            m_audioCh1VolumeAndEnvelope = value;
            break;
        }

        // $FF13
        case IoRegister::SoundCh1PeriodLow: {
            m_audioCh1PeriodLow = value;
            break;
        }

        // $FF14
        case IoRegister::SoundCh1PeriodHighControl: {
            m_audioCh1PeriodHighAndControl = value | 0b00111000;
            m_audioCh1Triggered = BIT_TEST(value, 7);
            break;
        }

        // $FF16
        case IoRegister::SoundCh2LengthDuty: {
            m_audioCh2LengthAndDutyCycle = value | 0b00111111;
            break;
        }

        // $FF17
        case IoRegister::SoundCh2VolumeEnvelope: {
            m_audioCh2VolumeAndEnvelope = value;
            break;
        }

        // $FF18
        case IoRegister::SoundCh2PeriodLow: {
            m_audioCh2PeriodLow = value;
            break;
        }

        // $FF19
        case IoRegister::SoundCh2PeriodHighControl: {
            m_audioCh2PeriodHighAndControl = value | 0b00111000;
            m_audioCh2Triggered = BIT_TEST(value, 7);
            break;
        }

        // $FF1A
        case IoRegister::SoundCh3DacEnable: {
            m_audioCh3DacEnable = BIT_TEST(value, 7);
            break;
        }

        // $FF1B
        case IoRegister::SoundCh3Length: {
            m_audioCh3LengthTimer = value;
            break;
        }

        // $FF1C
        case IoRegister::SoundCh3OutputLevel: {
            m_audioCh3OutputLevel = (value & 0b01100000) >> 5;
            break;
        }

        // $FF1D
        case IoRegister::SoundCh3PeriodLow: {
            m_audioCh3PeriodLow = value;
            break;
        }

        // $FF1E
        case IoRegister::SoundCh3PeriodHighControl: {
            m_audioCh3PeriodHighAndControl = value & 0b11000111;
            m_audioCh3Triggered = BIT_TEST(value, 7);
            break;
        }

        // $FF20
        case IoRegister::SoundCh4Length: {
            m_audioCh4LengthTimer = value & 0b00111111;
            break;
        }

        // $FF21
        case IoRegister::SoundCh4VolumeEnvelope: {
            m_audioCh4VolumeAndEnvelope = value;
            break;
        }

        // $FF22
        case IoRegister::SoundCh4FrequencyRandomness: {
            m_audioCh4FrequencyAndRandomness = value;
            break;
        }

        // $FF23
        case IoRegister::SoundCh4Control: {
            m_audioCh4Control = value & 0b11000000;
            m_audioCh4Triggered = BIT_TEST(value, 7);
            break;
        }

        // $FF24
        case IoRegister::SoundVolume: {
            if(!m_audioEnabled) break;

            m_audioMasterVolume = value & 0b01110111;
            m_audioVinPanning = value & 0b10001000;
            break;
        }

        // $FF25
        case IoRegister::SoundPanning: {
            if(m_audioEnabled) m_audioChannelPanning = value;
            break;
        }

        // $FF26
        case IoRegister::SoundEnable: {
            if((m_audioEnabled = BIT_TEST(value, 7)) == false) {
                Audio_ResetRegisters();
            }
            break;
        }

        // $FF40
        case IoRegister::LcdControl: {
            m_regLcdControl = value;
            break;
        }

        // $FF41
        case IoRegister::LcdStatus: {
            m_regLcdStatus = PadValue(value & 0b11111000, 7);
            break;
        }

        // $FF42
        case IoRegister::ScrollY: {
            m_regScrollY = value;
            break;
        }

        // $FF43
        case IoRegister::ScrollX: {
            m_regScrollX = value;
            break;
        }

        // $FF44 - IoRegister::LcdYCoordinate (read-only)

        // $FF45
        case IoRegister::LcdYCompare: {
            m_regLcdYCompare = value;
            break;
        }

        // $FF46
        case IoRegister::OamDmaSourceAddress: {
            m_videoPerformOamDmaCopy = true;
            m_regOamDmaAddress = value;
            break;
        }

        // $FF47
        case IoRegister::BgPalette: {
            m_regDmgBgPalette = value;
            break;
        }

        // $FF48
        case IoRegister::Obj0Palette: {
            m_regDmgObj0Palette = value;
            break;
        }

        // $FF49
        case IoRegister::Obj1Palette: {
            m_regDmgObj1Palette = value;
            break;
        }

        // $FF4A
        case IoRegister::WindowY: {
            m_regWindowY = value;
            break;
        }

        // $FF4B
        case IoRegister::WindowX: {
            m_regWindowX = value;
            break;
        }
        
        // $FF50
        case IoRegister::BootRomDisable: {
            if(!m_bootRomDisabled) {
                m_bootRomDisabled = value > 0;
            }
            break;
        }

        default: break;
    }

    if(CGB_SUPPORT) {
        switch(ioRegister) {
            // $FF4C
            case IoRegister::CpuModeSelect: {
                if(!m_bootRomDisabled) {
                    m_dmgCompatibility = BIT_TEST(value, 2);
                }
                break;
            }

            // $FF4D
            case IoRegister::SpeedSwitch: {
                m_speedSwitchArmed = BIT_TEST(value, 0);
                break;
            }

            // $FF4F
            case IoRegister::VramBank: {
                m_videoPerformVideoRamBankSwitch = m_regVramBank = value & 0b1;
                break;
            }

            // $FF51
            case IoRegister::VramDmaSourceHigh: {
                m_videoHdmaSourceAddress = (value << 8) | (m_videoHdmaSourceAddress & 0xFF);
                break;
            }

            // $FF52
            case IoRegister::VramDmaSourceLow: {
                m_videoHdmaSourceAddress = (m_videoHdmaSourceAddress & 0xFF00) | (value & 0xF0);
                break;
            }

            // $FF53
            case IoRegister::VramDmaDestinationHigh: {
                m_videoHdmaDestinationAddress = ((value & 0x0F) << 8) | (m_videoHdmaDestinationAddress & 0xFF);
                break;
            }

            // $FF54
            case IoRegister::VramDmaDestinationLow: {
                m_videoHdmaDestinationAddress = (m_videoHdmaDestinationAddress & 0xFF00) | (value & 0xF0);
                break;
            }

            // $FF55
            case IoRegister::VramDmaLengthModeStart: {
                if(m_videoHdmaTransferMode == DmaTransferMode::HBlank && BIT_TEST(value, 7)) {
                    // Writing to $FF55 bit 7 while performing an HBlank transfer will abort the transfer.
                    m_videoHdmaTransferMode = DmaTransferMode::Inactive;
                    m_videoHdmaTransferCancelled = true;
                } else {
                    m_videoHdmaTransferMode = BIT_TEST(value, 7) ? DmaTransferMode::HBlank : DmaTransferMode::GeneralPurpose;
                    m_videoHdmaTransferLength = (value & 0b01111111) << 4;
                }
                break;
            }

            // $FF68
            case IoRegister::BackgroundPaletteIndex: {
                m_videoBgPaletteIndex = value & 0b111111;
                m_videoBgPaletteAutoIncrement = BIT_TEST(value, 7);
                break;
            }

            // $FF69
            case IoRegister::BackgroundPaletteData: {
                m_videoCgbBgPaletteRam->SetByte(m_videoBgPaletteIndex, value | 0b1);
                if(m_videoBgPaletteAutoIncrement) {
                    m_videoBgPaletteIndex = (m_videoBgPaletteIndex + 1) & 0b111111;
                }
                break;
            }

            // $FF6A
            case IoRegister::ObjectPaletteIndex: {
                m_videoObjPaletteIndex = value & 0b111111;
                m_videoObjPaletteAutoIncrement = BIT_TEST(value, 7);
                break;
            }

            // $FF6B
            case IoRegister::ObjectPaletteData: {
                m_videoCgbObjPaletteRam->SetByte(m_videoObjPaletteIndex, value | 0b1);
                if(m_videoObjPaletteAutoIncrement) {
                    m_videoObjPaletteIndex = (m_videoObjPaletteIndex + 1) & 0b111111;
                }
                break;
            }

            // $FF6C
            case IoRegister::ObjectPriorityMode: {
                m_videoCgbObjectPriority = !BIT_TEST(value, 0);
                break;
            }
        
            // $FF70
            case IoRegister::WramBank: {
                m_performWorkRamBankSwitch = m_regWramBank = value & 0b111;
                break;
            }

            default: break;
        }
    }
}

void GameBoyIoRegisters::Timer_Cycle() {
    // Perform TIMA reload if necessary.
    if(m_timerTimaReloadStatus == ReloadScheduled) {
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Timer);
        m_regTimerCounter = m_regTimerModulo;
        m_timerTimaReloadStatus = ReloadJustOccurred;
    } else if(m_timerTimaReloadStatus == ReloadJustOccurred) {
        m_timerTimaReloadStatus = NoReload;
    }

    // Either reset or increment the timer.
    m_timerInternal += 4;  // 4 T-cycles per M-cycle

    // Work out which bit should potentially increment TIMA.
    const auto timerControl = m_regTimerControl;
    const auto timaEnabled = timerControl & 0b100;
    const auto timaClock = timerControl & 0b11;
    
    const auto frequencyBit = Timer_GetFrequencyBit(timaClock);
    const bool thisBitResult = ((m_timerInternal >> frequencyBit) & 0b1) && timaEnabled;

    Timer_FallingEdgeDetection(thisBitResult);
    Audio_DivApuIncrementCheck();
}

void GameBoyIoRegisters::Timer_FallingEdgeDetection(const bool thisBit) {
    if(!thisBit && m_timerLastBitResult) {
        if(++m_regTimerCounter == 0) {
            m_timerTimaReloadStatus = ReloadScheduled;
        }
    }
    m_timerLastBitResult = thisBit;
}

int GameBoyIoRegisters::Timer_GetFrequencyBit(const int clockSelect) {
    switch(clockSelect) {
        case 0b00: return 9;
        case 0b01: return 3;
        case 0b10: return 5;
        case 0b11: return 7;
        default: throw PlipEmulationException("Invalid timer clock.");
    }
}
