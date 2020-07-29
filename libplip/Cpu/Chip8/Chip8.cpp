/* Chip8.cpp
 *
 * An implementation of a CHIP-8 CPU.
 */

#include <iomanip>
#include <sstream>

#include "Chip8.h"
#include "../../PlipUtility.h"

namespace Plip::Cpu {
    Chip8::Chip8(long hz, PlipMemoryMap *memoryMap, uint16_t charset, Plip::PlipInput *input)
    : PlipCpu(hz, memoryMap) {
        m_charsetAddress = charset;
        m_input = input;
        m_videoBuffer = new uint64_t[VideoSize] {};
    }

    void Chip8::Cycle() {
        auto inst = Fetch();
        auto left = GetReg1(inst);
        auto right = GetReg2(inst);
        auto val = GetValue(inst);
        auto addr = GetAddress(inst);

        if(m_waitForKey) {
            for(uint8_t i = 0; i < 0x10; i++) {
                if(!m_input->GetInput(i).digital) continue;

                m_waitForKey = false;
                m_reg[m_keyRegister] = i;
                break;
            }
        }

        if(m_waitForKey) return;

        switch(inst & 0xF000) {
            case 0x0000:
                if(val == 0xE0)
                    Op00E0();
                else if(val == 0xEE)
                    Op00EE();
                else
                    Op0NNN(addr);
                break;

            case 0x1000: Op1NNN(addr); break;
            case 0x2000: Op2NNN(addr); break;
            case 0x3000: Op3XNN(left, val); break;
            case 0x4000: Op4XNN(left, val); break;
            case 0x5000: Op5XY0(left, right); break;
            case 0x6000: Op6XNN(left, val); break;
            case 0x7000: Op7XNN(left, val); break;
            case 0x8000: Op8XYO(left, right, inst & 0xF); break;
            case 0x9000: Op9XY0(left, right); break;
            case 0xA000: OpANNN(addr); break;
            case 0xB000: OpBNNN(addr); break;
            case 0xC000: OpCXNN(left, val); break;
            case 0xD000: OpDXYN(left, right, inst & 0xF); break;
            case 0xE000: OpEXOO(left, val); break;
            case 0xF000: OpFXOO(left, val); break;
        }
    }

    void Chip8::DelayTimer() {
        if(m_timerAudio > 0) m_timerAudio--;
        if(m_timerDelay > 0) m_timerDelay--;
    }

    std::string Chip8::DumpRegisters() {
        using util = Plip::PlipUtility;
        const char *regLabel = "0123456789ABCDEF";
        std::stringstream dump;

        dump << util::DumpValue("   PC", m_pc, 3) << '\n'
             << util::DumpValue("   SP", m_sp, StackSize > 16 ? 2 : 1) << "\n\n"
             << util::DumpValue("Audio", m_timerAudio, 2) << '\n'
             << util::DumpValue("Delay", m_timerDelay, 2) << "\n\n"
             << util::DumpValue("    I", m_i, 4) << '\n';

        for(auto i = 0; i < 16; i++) {
            std::stringstream label;
            label << "   V" << regLabel[i];
            dump << util::DumpValue(label.str(), m_reg[i], 2) << '\n';
        }

        dump << "\n\tStack: ";
        for(auto i = 0; i < StackSize; i++) {
            if(i == m_sp) dump << "[";
            dump << util::FormatHex(m_stack[i], 3);
            if(i == m_sp) dump << "]";
            dump << ' ';
        }

        return dump.str();
    }

    void Chip8::Reset(uint32_t pc) {
        m_timerAudio = 0;
        m_timerDelay = 0;
        m_sp = 0;
        m_pc = pc;
        m_i = 0;

        for(auto &reg : m_reg)
            reg = 0;

        for(auto &stack : m_stack)
            stack = 0;
    }
}
