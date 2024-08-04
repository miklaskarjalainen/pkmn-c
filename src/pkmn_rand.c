// TODO: Write a custom random machine.

#include <stdlib.h>

#include "pkmn_rand.h"

void pkmn_init_seed(uint32_t seed) {
    srand(seed);
}

uint8_t  pkmn_rand_u8(void) {
    return (uint8_t)rand();
}

uint32_t pkmn_rand_u32(void) {
    return (uint32_t)rand();
}

int32_t pkmn_rand_i32(void) {
    return (int32_t)rand();
}

int32_t pkmn_rand_range(int32_t min, int32_t max) {
    return pkmn_rand_i32() % (max + 1 - min) + min;
}

float pkmn_randf(void) {
    return (float)pkmn_rand_range(0, 65536) / 65536.0f;
}

