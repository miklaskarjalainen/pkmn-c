#ifndef PKMN_RAND_H
#define PKMN_RAND_H

#include <stdint.h>

void pkmn_init_seed(uint32_t seed);
uint8_t pkmn_rand_u8(void);
uint16_t pkmn_rand_u16(void);
uint32_t pkmn_rand_u32(void);
int32_t pkmn_rand_i32(void);
int32_t pkmn_rand_range(int32_t min, int32_t max);
float pkmn_randf(void);

#endif

