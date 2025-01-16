/* PlipSupport.h
 *
 * Macros that are useful to all Plip subsystems.
 */

#pragma once

#define BIT_CLEAR(var, bit) var &= ~(1 << (bit))
#define BIT_FLIP(var, bit) var ^= (1 << (bit))
#define BIT_SET(var, bit) var |= (1 << (bit))
#define BIT_TEST(var, bit) (var & (1 << (bit)))
