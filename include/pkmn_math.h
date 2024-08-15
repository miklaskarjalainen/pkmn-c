#ifndef PKMN_MATH_H
#define PKMN_MATH_H

// Reimplementation of some math.h functions, because i don't want to link with libc.

#include <stdint.h>

float pkmn_inverse_sqrt(float number);
float pkmn_sqrt(float number);
uint32_t pkmn_pow(uint32_t base, uint32_t power);

#endif