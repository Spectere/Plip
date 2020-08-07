/* PlipMacros.h
 *
 * Macros that are useful to all Plip subsystem.s
 */

#pragma once

namespace Plip {
    enum class ScheduledState {
        Disabled,
        Enabled,
        Scheduled
    };
}

#define BIT_CLEAR(var, bit) var &= ~(1 << (bit))
#define BIT_FLIP(var, bit) var ^= (1 << (bit))
#define BIT_SET(var, bit) var |= (1 << (bit))
#define BIT_TEST(var, bit) (var & (1 << (bit)))

#define COMBINE16LE(high, low) (((high) << 8) | (low))
