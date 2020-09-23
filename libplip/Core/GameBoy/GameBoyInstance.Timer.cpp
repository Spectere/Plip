/* GameBoyInstance.timer.cpp
 *
 * Simulated timer implementation in the GameBoy core.
 */

#include "GameBoyInstance.h"

namespace Plip::Core::GameBoy {
    void GameBoyInstance::TimerDividerTick() {
        if(m_lastWrite.address == m_addrRegisters + m_regDivider) {
            // Falling edge detector quirk.
            m_divFallingEdge = BIT_TEST(m_divider, 0);

            // Reset timer.
            m_divider = m_dividerTick = 0;
            m_ioRegisters->SetByte(m_regDivider, m_divider);
        } else {
            m_dividerTick += 4;
            if(m_dividerTick == 0) {
                // Increment divider when the divider tick wraps around
                // (4194304 / 16384 == 256).
                m_divider++;
                m_ioRegisters->SetByte(m_regDivider, m_divider);
            }
        }
    }

    void GameBoyInstance::TimerExecute() {
        // Per-Cycle Initialization
        m_divFallingEdge = false;

        // Divider
        TimerDividerTick();

        // TIMA
        m_tacLast = m_tac;
        m_tac = m_ioRegisters->GetByte(m_regTac);
        if(BIT_TEST(m_tac, 2)) {
            TimerTimaCycle();
        }
    }

    void GameBoyInstance::TimerTimaCycle() {
        m_timerIntBlocked = false;

        // Check for a scheduled interrupt.
        if(m_timerIntScheduled) {
            auto tma = m_ioRegisters->GetByte(m_regTma);
            m_timer = tma;
            m_ioRegisters->SetByte(m_regTima, m_timer);
            m_cpu->Interrupt(INTERRUPT_TIMER);
            m_timerIntScheduled = false;
        }

        // If the falling edge detector triggered on DIV, increment TIMA.
        if(m_divFallingEdge) TimerTimaIncrement();

        if(m_lastWrite.address == m_addrRegisters + m_regTima)
            m_timerIntBlocked = true;

        if(m_lastWrite.address == m_addrRegisters + m_regTac) {
            // If the falling edge detector triggered on TAC, increment TIMA.
            if(((m_tacLast & 0b11) == 0b01) && ((m_tac & 0b11) == 0b00))
                TimerTimaIncrement();
        }

        // Increment internal timer and increment TIMA if necessary.
        ++m_timerTick;
        switch(m_tac & 0b11) {
            case 0b00:  // 4096hz / 1024 clocks / 256 mcycles
                if(m_timerTick == 0)
                    TimerTimaIncrement();
                break;
            case 0b01:  // 262144hz / 16 clocks / 4 mcycles
                if(m_timerTick % 4 == 0)
                    TimerTimaIncrement();
                break;
            case 0b10:  // 65536hz / 64 clocks / 16 mcycles
                if(m_timerTick % 16 == 0)
                    TimerTimaIncrement();
                break;
            case 0b11:  // 16384hz / 256 clocks / 64 mcycles
                if(m_timerTick % 64 == 0)
                    TimerTimaIncrement();
                break;
        }
    }

    inline void GameBoyInstance::TimerTimaIncrement() {
        // TIMA was written to before the interrupt could be fired.
        if(m_timerIntBlocked) return;

        m_timer = m_ioRegisters->GetByte(m_regTima);

        if(++m_timer == 0) {
            // Timer overflowed. Schedule an interrupt.
            m_timerIntScheduled = true;
            m_ioRegisters->SetByte(m_addrRegisters + m_regTima, m_timer);
        }
    }
}
