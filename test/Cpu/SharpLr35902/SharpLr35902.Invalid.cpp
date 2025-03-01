/* SharpLr35902.Bits.cpp
 *
 * Tests the CPU core's lack of support for invalid opcodes. :^)
 */

#include "PlipInvalidOpcodeException.h"
#include "catch2/catch_test_macros.hpp"

#include "SharpLr35902Common.h"

TEST("0xD3", "0xD3") {
    LoadData(0x00, { 0xD3 });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xDB", "0xDB") {
    LoadData(0x00, { 0xDB });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xDD", "0xDD") {
    LoadData(0x00, { 0xDD });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xE3", "0xE3") {
    LoadData(0x00, { 0xE3 });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xE4", "0xE4") {
    LoadData(0x00, { 0xE4 });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xEB", "0xEB") {
    LoadData(0x00, { 0xEB });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xEC", "0xEC") {
    LoadData(0x00, { 0xEC });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xED", "0xED") {
    LoadData(0x00, { 0xED });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xF4", "0xF4") {
    LoadData(0x00, { 0xF4 });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xFC", "0xFC") {
    LoadData(0x00, { 0xFC });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}

TEST("0xFD", "0xFD") {
    LoadData(0x00, { 0xFD });
    REQUIRE_THROWS_AS(cpu->Step(), PlipInvalidOpcodeException);
}
