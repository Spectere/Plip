/* SharpLr35902.Ops.cpp
 *
 * An implementation of a Sharp LR35902 CPU (SM83 core).
 * This file implements the decode and execution steps.
 */

#include "SharpLr35902.h"

using Plip::Cpu::SharpLr35902;

long SharpLr35902::Cycle() {
    return m_cycle;
}

