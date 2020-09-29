/* GameBoyInstance.timer.cpp
 *
 * Simulated timer implementation in the GameBoy core.
 */

#include "GameBoyInstance.h"

#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

namespace Plip::Core::GameBoy {
    void GameBoyInstance::TimerExecute() {
        m_timerLast = m_timer;
        if(m_lastWrite.address == m_addrRegisters + m_regDivider) {
            // Reset the timer variable if DIV is written to.
            m_timer = 0;
        }

        // The timer increases by 4 every cycle.
        m_timer += 4;

        if(m_timerTimaOverflow) {
            // Reload TIMA and raise an interrupt (if applicable).
            auto tma = m_ioRegisters->GetByte(m_regTma);
            m_ioRegisters->SetByte(m_regTima, tma);

            // Quirk: If IF is written during the interrupt cycle, the
            // written value will override this.
            if(m_lastWrite.address == Plip::Cpu::SharpLr35902::MemInterruptFlag)
                m_cpu->Interrupt(INTERRUPT_TIMER);
        }

        // Handle TIMA.
        TimerTima();

        // Update the divider register.
        m_ioRegisters->SetByte(m_regDivider, m_timer >> 8);
    }

    inline uint8_t GameBoyInstance::TimerFallingEdgeBit(uint8_t tac) {
        switch(tac & 0b11) {
            case 0b00:
                return 9;
            case 0b01:
                return 3;
            case 0b10:
                return 5;
            case 0b11:
                return 7;
        }

        return 0;  // Compiler appeasement. :)
    }

    inline bool GameBoyInstance::TimerFallingEdgeDetection(uint8_t bit) const {
        return BIT_TEST(m_timerLast, bit) && !BIT_TEST(m_timer, bit);
    }

    inline void GameBoyInstance::TimerIncreaseTima() {
        uint8_t tima = m_ioRegisters->GetByte(m_regTima) + 1;
        m_ioRegisters->SetByte(m_regTima, tima);

        if(tima == 0) {
            // TIMA overflow.
            m_timerTimaOverflow = true;
        }
    }

    void GameBoyInstance::TimerTima() {
        auto tac = m_ioRegisters->GetByte(m_regTac) & 0b111;
        auto freqBit = TimerFallingEdgeBit(tac);
        m_timerTimaOverflow = false;

        if(TimerFallingEdgeDetection(freqBit) && BIT_TEST(tac, 2)) {
            TimerIncreaseTima();
        } else if(BIT_TEST(m_timer, freqBit) && BIT_TEST(m_timerTacLast, 2) && !BIT_TEST(tac, 2)) {
            // Quirk: Increase TIMA if the corresponding bit is set when disabling
            // the timer.
            TimerIncreaseTima();
        } else if(m_lastWrite.address == m_addrRegisters + m_regTac
            && !BIT_TEST(m_timerTacLast, 2) && BIT_TEST(tac, 2)
            && (m_timerTacLast & 0b11) == 0 && (tac & 0b11) == 1) {
            // Quirk: Increase TIMA if the timer goes from disabled to enabled, and
            // the multiplexer goes from 0 to 1 (agh).
            TimerIncreaseTima();
        }

        if(m_lastWrite.address == m_addrRegisters + m_regTima && m_timerTimaOverflow) {
            // Quirk: If TIMA is written to during the cycle that causes it to
            // overflow, the pending reset and interrupt are cancelled.
            m_timerTimaOverflow = false;
            m_ioRegisters->SetByte(m_regTima, m_lastWrite.value);
        }

        m_timerTacLast = tac;

        // Write TAC back into memory to ensure that only the low 3 bits are set.
        m_ioRegisters->SetByte(m_regTac, tac);
    }
}
