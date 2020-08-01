/* SharpLr35902.h
 *
 * An implementation of the Sharp LR35902 (GameBoy) CPU.
 */

#pragma once

#include <string>
#include <tuple>
#include <vector>

#include "../PlipCpu.h"
#include "../../Memory/PlipMemoryMap.h"

namespace Plip::Cpu {
    class SharpLr35902 : PlipCpu {
    public:
        SharpLr35902(long hz, PlipMemoryMap* memoryMap, uint32_t pc);

        struct Registers {
            uint8_t a;
            uint8_t f;
            uint8_t b;
            uint8_t c;
            uint8_t d;
            uint8_t e;
            uint8_t h;
            uint8_t l;

            uint16_t sp;
            uint16_t pc;
        };

        void Cycle() override;
        Registers GetRegisters() { return m_reg; }
        void Reset() override;

    private:
        void PerformReset();

        void Decode();
        void DecodeCB();
        [[nodiscard]] std::string DumpRegisters() const;
        uint8_t* GetRegister8(uint8_t idx);
        std::tuple<uint8_t*, uint8_t*> GetRegisterPair(uint8_t idx);
        uint16_t GetAddress(uint8_t idx);

        static inline uint16_t Combine(const uint8_t *high, const uint8_t *low) {
            return ((*high << 8) + *low);
        }

        static inline void Split(uint16_t val, uint8_t *high, uint8_t *low) {
            *high = val >> 8;
            *low = val & 0xFF;
        }

        static inline void DecPair(uint8_t *high, uint8_t *low, uint16_t amount = 1) {
            uint16_t val = Combine(high, low) - amount;
            Split(val, high, low);
        }

        static inline void IncPair(uint8_t *high, uint8_t *low, uint16_t amount = 1) {
            uint16_t val = Combine(high, low) + amount;
            Split(val, high, low);
        }

        // Standard opcodes.
        void OpDecReg();
        void OpDecPair();
        void OpIncReg();
        void OpIncPair();
        void OpLdMemReg();
        void OpLdRegImm();
        void OpLdRegMem();
        void OpLdRegReg();

        // CB-prefixed opcodes.
        void OpBitClear();
        void OpBitSet();
        void OpBitTest();

        bool m_allowFetch = true;
        std::vector<uint8_t> m_instr;
        Registers m_reg {};
        uint8_t m_mcycle = 0;
    };
}
