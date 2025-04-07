/* GameBoyIoRegisters.cpp
 *
 * Implements the Game Boy's I/O registers.
 */

#include "GameBoyIoRegisters.h"

#include "../../PlipSupport.h"

using Plip::Core::GameBoy::GameBoyIoRegisters;

GameBoyIoRegisters::GameBoyIoRegisters() {
    Reset();
}

uint8_t GameBoyIoRegisters::GetByte(const IoRegister ioRegister) const {
    switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
        /* $FF00 */ case IoRegister::JoypadInput: { return m_regJoypad; }
        /* $FF04 */ case IoRegister::Divider: { return m_timerRegister >> 8; }
        /* $FF05 */ case IoRegister::TimerCounter: { return m_regTimerCounter; }
        /* $FF06 */ case IoRegister::TimerModulo: { return m_regTimerModulo; }
        /* $FF07 */ case IoRegister::TimerControl: { return m_regTimerControl; }
        /* $FF0F */ case IoRegister::InterruptFlag: { return m_interruptFlag; }
        /* $FF40 */ case IoRegister::LcdControl: { return m_regLcdControl; }
        /* $FF41 */ case IoRegister::LcdStatus: { return m_regLcdStatus; }
        /* $FF42 */ case IoRegister::ScrollY: { return m_regScrollY; }
        /* $FF43 */ case IoRegister::ScrollX: { return m_regScrollX; }
        /* $FF44 */ case IoRegister::LcdYCoordinate: { return m_regLcdYCoordinate; }
        /* $FF45 */ case IoRegister::LcdYCompare: { return m_regLcdYCompare; }
        /* $FF47 */ case IoRegister::BgPalette: { return m_regBgPalette; }
        /* $FF4A */ case IoRegister::WindowY: { return m_regWindowY; }
        /* $FF4B */ case IoRegister::WindowX: { return m_regWindowX; }
        /* $FF50 */ case IoRegister::BootRomDisable: { return m_bootRomDisabled ? 0b1 : 0b0; }

        default: { return 0b11111111; };
    }
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
    m_timerRegister = 0;
    m_regTimerCounter = 0;
    m_regTimerControl = 0xF8;
    m_regTimerModulo = 0;
}

void GameBoyIoRegisters::SetByte(const IoRegister ioRegister, const uint8_t value) {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    // ReSharper disable once CppIncompleteSwitchStatement
    switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
        // $FF00
        case IoRegister::JoypadInput: {
            m_regJoypad = value;
            break;
        }

        // $FF04
        case IoRegister::Divider: {
            m_timerDividerReset = true;
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

        // $FF47
        case IoRegister::BgPalette: {
            m_regBgPalette = value;
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
    }
}

void GameBoyIoRegisters::Timer_Cycle() {
    // Perform TIMA reload if necessary.
    if(m_timerTimaReloadStatus == ReloadScheduled) {
        RaiseInterrupt(Cpu::SharpLr35902Interrupt::Timer);
        m_regTimerControl = m_regTimerModulo;
        m_timerTimaReloadStatus = ReloadJustOccurred;
    } else if(m_timerTimaReloadStatus == ReloadJustOccurred) {
        m_timerTimaReloadStatus = NoReload;
    }

    // Either reset or increment the timer.
    if(m_timerDividerReset) {
        m_timerRegister = 0;
        m_timerDividerReset = false;
    } else {
        m_timerRegister += 4;  // 4 T-cycles per M-cycle
    }

    // Work out which bit should potentially increment TIMA.
    const auto timerControl = m_regTimerControl;
    const auto timaEnabled = timerControl & 0b100;
    const auto timaClock = timerControl & 0b11;
    
    const auto frequencyBit = Timer_GetFrequencyBit(timaClock);
    const bool thisBitResult = ((m_timerRegister >> frequencyBit) & 0b1) && timaEnabled;

    Timer_FallingEdgeDetection(thisBitResult);
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
