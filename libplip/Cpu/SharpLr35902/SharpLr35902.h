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
        SharpLr35902(long hz, PlipMemoryMap* memoryMap);

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
        enum ImeState {
            Disabled,
            Enabled,
            Scheduled
        };

        void PerformReset();

        void Decode();
        void DecodeCB();
        [[nodiscard]] std::string DumpRegisters() const;
        uint8_t* GetRegister8(uint8_t idx);
        std::tuple<uint8_t*, uint8_t*> GetRegisterPair(uint8_t idx);
        uint16_t GetRegister16Value(uint8_t idx);
        bool TestConditional(uint8_t idx);

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
        void OpAccumAddImm();
        void OpAccumAddCarryImm();
        void OpAccumAndImm();
        void OpAccumBcd();
        void OpAccumCarryImm();
        void OpAccumFlip();
        void OpAccumOrImm();
        void OpAccumRotateLeft();
        void OpAccumRotateLeftThruCarry();
        void OpAccumRotateRight();
        void OpAccumRotateRightThruCarry();
        void OpAccumSubImm();
        void OpAccumSubBorrowImm();
        void OpAccumXorImm();
        void OpAdd();
        void OpAdd16();
        void OpAddCarry();
        void OpAddSpReg();
        void OpAnd();
        void OpCallCond();
        void OpCallUnc();
        void OpCarry();
        void OpDecReg();
        void OpDecPair();
        void OpDisableInterrupts();
        void OpEnableInterrupts();
        void OpFlipCarry();
        void OpFuncFixedUnc();
        void OpIncReg();
        void OpIncPair();
        void OpJumpAbsCond();
        void OpJumpAbsUnc();
        void OpJumpRegUnc();
        void OpJumpRelCond();
        void OpJumpRelUnc();
        void OpLdAccumMem();
        void OpLdAccumMemHigh();
        void OpLdAccumMemHighC();
        void OpLdHlSpOffset();
        void OpLdMemAccum();
        void OpLdMemHighAccum();
        void OpLdMemHighCAccum();
        void OpLdMemReg();
        void OpLdMemSp();
        void OpLdReg16Imm16();
        void OpLdRegImm();
        void OpLdRegMem();
        void OpLdRegReg();
        void OpLdSpHl();
        void OpOr();
        void OpPopReg16();
        void OpPushReg16();
        void OpRetCond();
        void OpRetUnc();
        void OpRetImeUnc();
        void OpSetCarry();
        void OpSub();
        void OpSubBorrow();
        void OpXor();

        // CB-prefixed opcodes.
        void OpBitClear();
        void OpBitSet();
        void OpBitTest();
        void OpNibbleSwap();
        void OpRotateLeft();
        void OpRotateLeftThruCarry();
        void OpRotateRight();
        void OpRotateRightThruCarry();
        void OpShiftLeftArithmetic();
        void OpShiftRightArithmetic();
        void OpShiftRightLogical();

        bool m_allowFetch = true;
        ImeState m_ime = Disabled;
        std::vector<uint8_t> m_instr;
        Registers m_reg {};
        uint8_t m_mcycle = 0;
    };
}
