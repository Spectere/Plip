/* SharpLr35902.Decode.cpp
 *
 * Sharp LR35902 instruction decoding.
 */

#include <sstream>

#include "../../PlipEmulationException.h"
#include "../../PlipUtility.h"

#include "SharpLr35902.h"
#include "SharpLr35902.Macros.h"

namespace Plip::Cpu {
    void SharpLr35902::Decode() {
        auto opcode = m_instr[0];

        if((opcode & 0b11000000) == 0b01000000) {
            // LD r, r' / LD r, (HL) / LD (HL), r
            OpLdRR();
        } else if((opcode & 0b11000111) == 0b00000110) {
            // LD r, n
            OpLdRN();
        } else {
            std::stringstream ex;
            ex << "unknown opcode: " << PlipUtility::FormatHex(opcode, 2)
               << "\n\n" << DumpRegisters();
            throw PlipEmulationException(ex.str().c_str());
        }
    }

    void SharpLr35902::DecodeCB() {

    }
}
