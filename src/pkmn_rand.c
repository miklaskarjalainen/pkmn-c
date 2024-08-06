#include "pkmn_rand.h"

// RANDOM BASED ON https://tasvideos.org/GameResources/GBA/PokemonGen3/RNG
static uint32_t sRandomSeed = 0x0; 

void pkmn_init_seed(uint32_t seed) {
    sRandomSeed = seed;
}

uint8_t pkmn_rand_u8(void) {
    return (uint8_t)pkmn_rand_u32();
}

uint16_t pkmn_rand_u16(void) {
    return (uint16_t)pkmn_rand_u32();
}

uint32_t pkmn_rand_u32(void) {
    const uint32_t MULTIPLIER = 0x41C64E6D;
    const uint32_t ADDER = 0x6073;
    sRandomSeed = sRandomSeed * MULTIPLIER + ADDER;
    return sRandomSeed;
}

int32_t pkmn_rand_i32(void) {
    uint32_t res = pkmn_rand_u32();
    return *(int32_t*)&res;
}

int32_t pkmn_rand_range(int32_t min, int32_t max) {
    return pkmn_rand_u32() % (max + 1 - min) + min;
}

float pkmn_randf(void) {
    return (float)pkmn_rand_u16() / 0xFFFF;
}

