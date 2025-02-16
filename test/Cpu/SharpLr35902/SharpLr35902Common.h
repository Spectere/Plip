/* SharpLr35902Common.h
 *
 * The test fixture for the Sharp LR35902 CPU.
 */

#pragma once

#include "Cpu/SharpLr35902/SharpLr35902.h"
#include "Memory/PlipMemoryMap.h"
#include "Memory/PlipMemoryRam.h"

using namespace Plip;

#define TEST_NAME(name) "[Sharp LR35902 CPU] " name
#define OP(op) "[lr35902cpu][" op "]"
#define TEST(name, op) TEST_CASE_METHOD(MockCpu<Cpu::SharpLr35902>, TEST_NAME(name), OP(op))
