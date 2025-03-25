/* GameBoyIoRegisters.cpp
 *
 * Implements the GameBoy's I/O registers.
 */

#include "GameBoyIoRegisters.h"

#include "../../PlipSupport.h"

using Plip::Core::GameBoy::GameBoyIoRegisters;

GameBoyIoRegisters::GameBoyIoRegisters() {
    Reset();
}

uint8_t GameBoyIoRegisters::GetByte(const IoRegister ioRegister) {
    switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
        /* $FF00 */ case IoRegister::JoypadInput: { return m_regJoypad; }
        /* $FF04 */ case IoRegister::Divider: { return m_regDivider; }
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

void GameBoyIoRegisters::Reset() {
    m_bootRomDisabled = false;
    m_interruptFlag = 0;

    // Timer
    m_regDivider = 0;
    m_regTimerCounter = 0;
    m_regTimerControl = 0xF8;
    m_regTimerModulo = 0;
}

void GameBoyIoRegisters::SetByte(const IoRegister ioRegister, const uint8_t value) {
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    switch(ioRegister) {  // NOLINT(*-multiway-paths-covered)
        // $FF00
        case IoRegister::JoypadInput: {
            if(!BIT_TEST(value, 5)) {
                // Buttons
                m_regJoypad = PadValue(m_inputsPressed, 4);
            } else if(!BIT_TEST(value, 4)) {
                // D-pad
                m_regJoypad = PadValue(m_inputsPressed >> 4, 4);
            }
            break;
        }

        // $FF04
        case IoRegister::Divider: {
            m_regDivider = 0;
            break;
        }

        // $FF05
        case IoRegister::TimerCounter: {
            m_regTimerCounter = value;
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
            m_regLcdStatus = PadValue(value, 7);
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
