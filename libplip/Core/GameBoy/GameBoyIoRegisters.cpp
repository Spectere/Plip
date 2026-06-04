/* GameBoyIoRegisters.cpp
 *
 * Implements the Game Boy's I/O registers.
 */

#include "GameBoyIoRegisters.h"

#include "../../PlipSupport.h"

using Plip::Core::GameBoy::GameBoyIoRegisters;

#define CGB_SUPPORT (m_model == GameBoyModel::CGB)

GameBoyIoRegisters::GameBoyIoRegisters(const GameBoyModel gbModel, PlipMemory* cgbBgPaletteRam, PlipMemory* cgbObjPaletteRam)
                                     : m_model(gbModel), m_videoCgbBgPaletteRam(cgbBgPaletteRam), m_videoCgbObjPaletteRam(cgbObjPaletteRam) {
    Reset();
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
        /* $FF10 */ case IoRegister::SoundCh1Sweep: { return (m_audioPulse1.Ch1SweepPace << 4) | (m_audioPulse1.Ch1SweepSubtract ? 0b1000 : 0) | m_audioPulse1.Ch1SweepStep; }
        /* $FF11 */ case IoRegister::SoundCh1LengthDuty: { return (m_audioPulse1.DutyCycle << 6) | 0b00111111; }
        /* $FF12 */ case IoRegister::SoundCh1VolumeEnvelope: { return (m_audioPulse1.InitialVolume << 4) | (m_audioPulse1.EnvelopeIncrease ? 0b1000 : 0) | m_audioPulse1.EnvelopeSweepPace; }
        /* $FF13 */ case IoRegister::SoundCh1PeriodLow: { return 0xFF; }  // write-only
        /* $FF14 */ case IoRegister::SoundCh1PeriodHighControl: { return (m_audioPulse1.TimerEnabled ? 0b1000000 : 0) | 0b10111111; }
        /* $FF16 */ case IoRegister::SoundCh2LengthDuty: { return (m_audioPulse2.DutyCycle << 6) | 0b00111111; }
        /* $FF17 */ case IoRegister::SoundCh2VolumeEnvelope: { return (m_audioPulse2.InitialVolume << 4) | (m_audioPulse2.EnvelopeIncrease ? 0b1000 : 0) | m_audioPulse2.EnvelopeSweepPace; }
        /* $FF18 */ case IoRegister::SoundCh2PeriodLow: { return 0xFF; }  // write-only
        /* $FF19 */ case IoRegister::SoundCh2PeriodHighControl: { return (m_audioPulse2.TimerEnabled ? 0b1000000 : 0) | 0b10111111; }
        /* $FF1A */ case IoRegister::SoundCh3DacEnable: { return (m_audioWave.Enabled ? 0b10000000 : 0) | 0b01111111; }
        /* $FF1B - write-only */
        /* $FF1C */ case IoRegister::SoundCh3OutputLevel: { return (m_audioWave.Volume << 5) | 0b10011111; }
        /* $FF1D */ case IoRegister::SoundCh3PeriodLow: { return 0xFF; }  // write-only
        /* $FF1E */ case IoRegister::SoundCh3PeriodHighControl: { return (m_audioWave.TimerEnabled ? 0b1000000 : 0) | 0b10111111; }
        /* $FF20 - write-only */
        /* $FF21 */ case IoRegister::SoundCh4VolumeEnvelope: { return (m_audioNoise.InitialVolume << 4) | (m_audioNoise.EnvelopeIncrease ? 0b1000 : 0) | m_audioNoise.EnvelopeSweepPace; }
        /* $FF22 */ case IoRegister::SoundCh4FrequencyRandomness: { return (m_audioNoise.ClockShift << 4) | (m_audioNoise.LFSRWidth7Bit ? 0b1000 : 0) | m_audioNoise.ClockDivider; }
        /* $FF23 */ case IoRegister::SoundCh4Control: { return (m_audioNoise.TimerEnabled ? 0b1000000 : 0) | 0b10111111; }
        /* $FF24 */ case IoRegister::SoundVolume: { return m_audioVinPanning | m_audioMasterVolume; }
        /* $FF25 */ case IoRegister::SoundPanning: { return Audio_GetChannelPanning(); }
        /* $FF26 */ case IoRegister::SoundEnable: { return (m_audioEnabled ? 0b10000000 : 0) | Audio_GetChannelState() | 0b01110000; }
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
        /* $FF50 - write-only */

        default: break;
    }

    if(CGB_SUPPORT) {
        switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
            /* $FF4C */ case IoRegister::CpuModeSelect: { return !m_dmgCompatibility ? 0b11111011 : 0b11111111; }
            /* $FF4D */ case IoRegister::SpeedSwitch: { return 0b01111110 | (m_doubleSpeedActive ? 0b10000000 : 0) | (m_speedSwitchArmed ? 0b1 : 0); }
            /* $FF4F */ case IoRegister::VramBank: { return PadValue(m_regVramBank, 1); }
            /* $FF55 */ case IoRegister::VramDmaLengthModeStart: {
                return (m_videoHdmaTransferMode != DmaTransferMode::Inactive ? 0b10000000 : 0)
                     | ((m_videoHdmaTransferRemaining >> 4) & 0b01111111);
            }
            /* $FF68 */ case IoRegister::BackgroundPaletteIndex: { return (m_videoBgPaletteAutoIncrement ? 0b10000000 : 0) | 0b01000000 | (m_videoBgPaletteIndex & 0b111111); }
            /* $FF69 */ case IoRegister::BackgroundPaletteData: { return m_videoCgbBgPaletteRam->GetByte(m_videoBgPaletteIndex); }
            /* $FF6A */ case IoRegister::ObjectPaletteIndex: { return (m_videoObjPaletteAutoIncrement ? 0b10000000 : 0) | 0b01000000 | (m_videoObjPaletteIndex & 0b111111); }
            /* $FF6B */ case IoRegister::ObjectPaletteData: { return m_videoCgbObjPaletteRam->GetByte(m_videoObjPaletteIndex); }
            /* $FF6C */ case IoRegister::ObjectPriorityMode: { return 0xFF ^ (m_videoCgbObjectPriority ? 1 : 0); }
            /* $FF70 */ case IoRegister::WramBank: { return PadValue(m_regWramBank, 3); }

            default: break;
        }
    }

    return 0xFF;
}

uint8_t GameBoyIoRegisters::Audio_GetChannelPanning() const {
    return  m_audioPulse1.GetPanning()
         | (m_audioPulse2.GetPanning() << 2)
         | (m_audioWave.GetPanning()   << 4)
         | (m_audioNoise.GetPanning()  << 6);
}

uint8_t GameBoyIoRegisters::Audio_GetChannelState() const {
    return (m_audioPulse1.Enabled ? 0b0001 : 0)
         | (m_audioPulse2.Enabled ? 0b0010 : 0)
         | (m_audioWave.Enabled   ? 0b0100 : 0)
         | (m_audioNoise.Enabled  ? 0b1000 : 0);
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

            const auto thisBitResult = m_timerLastBitResult && m_regTimerControl & 0b100;
            Timer_FallingEdgeDetection(thisBitResult);

            break;
        }

        // $FF0F
        case IoRegister::InterruptFlag: {
            m_interruptFlag = PadValue(value, 5);
            break;
        }

        // $FF10
        case IoRegister::SoundCh1Sweep: {
            if(!m_audioEnabled) break;

            m_audioPulse1.Ch1SweepPace = (value & 0b01110000) >> 4;
            m_audioPulse1.Ch1SweepSubtract = value & 0b1000;
            m_audioPulse1.Ch1SweepStep = value & 0b111;
            break;
        }

        // $FF11
        case IoRegister::SoundCh1LengthDuty: {
            if(!m_audioEnabled) break;

            m_audioPulse1.DutyCycle = (value & 0b11000000) >> 6;
            m_audioPulse1.Length = ~(value & 0b111111) & 0b111111;
            break;
        }

        // $FF12
        case IoRegister::SoundCh1VolumeEnvelope: {
            if(!m_audioEnabled) break;

            m_audioPulse1.InitialVolume = (value & 0b11110000) >> 3;
            m_audioPulse1.EnvelopeIncrease = value & 0b1000;
            m_audioPulse1.EnvelopeSweepPace = value & 0b111;

            if((value & 0b11111000) == 0) {
                // Turn channel off.
                m_audioPulse1.Enabled = false;
            }

            break;
        }

        // $FF13
        case IoRegister::SoundCh1PeriodLow: {
            if(!m_audioEnabled) break;

            m_audioPulse1.Period = (m_audioPulse1.Period & 0xFF00) | value;
            break;
        }

        // $FF14
        case IoRegister::SoundCh1PeriodHighControl: {
            if(!m_audioEnabled) break;

            m_audioPulse1.TimerEnabled = value & 0b1000000;
            m_audioPulse1.Period = ((value & 0b111) << 8) | (m_audioPulse1.Period & 0xFF);

            if(value & 0b10000000) {  // Channel 1 trigger.
                m_audioPulse1.Enabled = true;
                if(m_audioPulse1.Length == 0) {
                    m_audioPulse1.Length = PulseChannel::MaxLength;
                }
                m_audioPulse1.Volume = m_audioPulse1.InitialVolume;
                m_audioPulse1.EnvelopeTimer = 0;
                m_audioPulse1.Volume = m_audioPulse1.InitialVolume;
                m_audioPulse1.PeriodDivider = m_audioPulse1.Period;

                // Special channel 1 sweep stuff.
                m_audioPulse1.Ch1SweepEnabled = m_audioPulse1.Ch1SweepPace || m_audioPulse1.Ch1SweepStep;
                m_audioPulse1.Ch1SweepTimer = 0;
                m_audioPulse1.Ch1PeriodShadow = m_audioPulse1.Period;
                m_audioPulse1.Ch1SweepPaceCurrent = m_audioPulse1.Ch1SweepPace;
                if(m_audioPulse1.Ch1SweepStep) {
                    // If sweep step is non-zero, force an immediate frequency calculation/overflow check.
                    m_audioPulse1.ClockSweep(0);
                }
            }
            break;
        }

        // $FF16
        case IoRegister::SoundCh2LengthDuty: {
            if(!m_audioEnabled) break;

            m_audioPulse2.DutyCycle = (value & 0b11000000) >> 6;
            m_audioPulse2.Length = ~(value & 0b111111) & 0b111111;
            break;
        }

        // $FF17
        case IoRegister::SoundCh2VolumeEnvelope: {
            if(!m_audioEnabled) break;

            m_audioPulse2.InitialVolume = (value & 0b11110000) >> 3;
            m_audioPulse2.EnvelopeIncrease = value & 0b1000;
            m_audioPulse2.EnvelopeSweepPace = value & 0b111;
            m_audioPulse2.PeriodDivider = m_audioPulse2.Period;

            if((value & 0b11111000) == 0) {
                // Turn channel off.
                m_audioPulse2.Enabled = false;
            }

            break;
        }

        // $FF18
        case IoRegister::SoundCh2PeriodLow: {
            if(!m_audioEnabled) break;

            m_audioPulse2.Period = (m_audioPulse2.Period & 0xFF00) | value;
            break;
        }

        // $FF19
        case IoRegister::SoundCh2PeriodHighControl: {
            if(!m_audioEnabled) break;

            m_audioPulse2.TimerEnabled = value & 0b1000000;
            m_audioPulse2.Period = ((value & 0b111) << 8) | (m_audioPulse2.Period & 0xFF);

            if(value & 0b10000000) {  // Channel 2 trigger.
                m_audioPulse2.Enabled = true;
                if(m_audioPulse2.Length == 0) {
                    m_audioPulse2.Length = PulseChannel::MaxLength;
                }
                m_audioPulse2.Volume = m_audioPulse2.InitialVolume;
                m_audioPulse2.EnvelopeTimer = 0;
                m_audioPulse2.Volume = m_audioPulse2.InitialVolume;
            }
            break;
        }

        // $FF1A
        case IoRegister::SoundCh3DacEnable: {
            if(!m_audioEnabled) break;

            m_audioWave.Enabled = true;
            break;
        }

        // $FF1B
        case IoRegister::SoundCh3Length: {
            if(!m_audioEnabled) break;

            m_audioWave.Length = ~value;
            break;
        }

        // $FF1C
        case IoRegister::SoundCh3OutputLevel: {
            if(!m_audioEnabled) break;

            m_audioWave.Volume = (value & 0b01100000) >> 4;
            break;
        }

        // $FF1D
        case IoRegister::SoundCh3PeriodLow: {
            if(!m_audioEnabled) break;

            m_audioWave.Period = (m_audioWave.Period & 0xFF00) | value;
            break;
        }

        // $FF1E
        case IoRegister::SoundCh3PeriodHighControl: {
            if(!m_audioEnabled) break;

            m_audioWave.TimerEnabled = value & 0b1000000;
            m_audioWave.Period = ((value & 0b111) << 8) | (m_audioWave.Period & 0xFF);

            if(value & 0b10000000) {  // Channel 3 trigger.
                m_audioWave.Enabled = true;
                if(m_audioWave.Length == 0) {
                    m_audioWave.Length = WaveChannel::MaxLength;
                }
                m_audioWave.PeriodDivider = m_audioWave.Period;
                m_audioWave.Volume = m_audioWave.InitialVolume;
                m_audioWave.WaveRamIndex = 0;
            }
            break;
        }

        // $FF20
        case IoRegister::SoundCh4Length: {
            if(!m_audioEnabled) break;

            m_audioNoise.Length = ~(value & 0b111111) & 0b111111;
            break;
        }

        // $FF21
        case IoRegister::SoundCh4VolumeEnvelope: {
            if(!m_audioEnabled) break;

            m_audioNoise.InitialVolume = (value & 0b11110000) >> 3;
            m_audioNoise.EnvelopeIncrease = value & 0b1000;
            m_audioNoise.EnvelopeSweepPace = value & 0b111;
            break;
        }

        // $FF22
        case IoRegister::SoundCh4FrequencyRandomness: {
            if(!m_audioEnabled) break;

            m_audioNoise.ClockShift = (value & 0b11110000) >> 4;
            m_audioNoise.LFSRWidth7Bit = value & 0b1000;
            m_audioNoise.ClockDivider = value & 0b111;
            break;
        }

        // $FF23
        case IoRegister::SoundCh4Control: {
            if(!m_audioEnabled) break;

            m_audioNoise.TimerEnabled = value & 0b1000000;

            if(value & 0b10000000) {  // Channel 4 trigger.
                m_audioNoise.Enabled = true;
                if(m_audioNoise.Length == 0) {
                    m_audioNoise.Length = NoiseChannel::MaxLength;
                }
                m_audioNoise.EnvelopeTimer = 0;
                m_audioNoise.Volume = m_audioNoise.InitialVolume;
                m_audioNoise.LFSRBits = 0;
            }
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
            if(!m_audioEnabled) break;

            m_audioPulse1.SetPanning(value & 0b00000011);
            m_audioPulse2.SetPanning(value & 0b00001100);
            m_audioWave  .SetPanning(value & 0b00110000);
            m_audioNoise .SetPanning(value & 0b11000000);

            break;
        }

        // $FF26
        case IoRegister::SoundEnable: {
            m_audioEnabled = BIT_TEST(value, 7);
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
                m_videoHdmaDestinationAddress = ((value & 0b11111) << 8) | (m_videoHdmaDestinationAddress & 0xFF);
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
                m_videoCgbBgPaletteRam->SetByte(m_videoBgPaletteIndex, value);
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
                m_videoCgbObjPaletteRam->SetByte(m_videoObjPaletteIndex, value);
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

void GameBoyIoRegisters::Timer_Cycle(const uint64_t mCycle) {
    // Perform TIMA reload if necessary.
    if(m_timerTimaReloadStatus == ReloadScheduled) {
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Timer);
        m_regTimerCounter = m_regTimerModulo;
        m_timerTimaReloadStatus = ReloadJustOccurred;
    } else if(m_timerTimaReloadStatus == ReloadJustOccurred) {
        if(mCycle != m_timerThisMCycle) {
            m_timerTimaReloadStatus = NoReload;
        }
    }

    m_timerThisMCycle = mCycle;

    // Either reset or increment the timer.
    m_timerInternal += 1;

    // Work out which bit should potentially increment TIMA.
    const auto timerControl = m_regTimerControl;
    const auto timaEnabled = timerControl & 0b100;
    const auto timaClock = timerControl & 0b11;

    const auto frequencyBit = Timer_GetFrequencyBit(timaClock);
    const bool thisBitResult = ((m_timerInternal >> frequencyBit) & 0b1) && timaEnabled;

    Timer_FallingEdgeDetection(thisBitResult);
    Timer_DivApuCheckIncrement();
}

void GameBoyIoRegisters::Timer_DivApuCheckIncrement() {
    // DIV-APU increments when bit 4 (or 5 in double-speed mode) falls (512 hz).
    const bool thisBit = m_timerInternal & (1 << (m_doubleSpeedActive ? AudioTimerBaseBit + 1 : AudioTimerBaseBit));

    if(!thisBit && m_timerDivApuLastBitResult) {
        ++m_audioDivApu;

        // Update each channel with the new value.
        m_audioPulse1.DivApuPulse(m_audioDivApu);
        m_audioPulse2.DivApuPulse(m_audioDivApu);
        m_audioWave.DivApuPulse(m_audioDivApu);
        m_audioNoise.DivApuPulse(m_audioDivApu);

        // Perform sweep calculations on channel 1.
        m_audioPulse1.ClockSweep(m_audioDivApu);
    }
    m_timerDivApuLastBitResult = thisBit;
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
