#include "pkmn_math.h"


float pkmn_inverse_sqrt(float number) {
	uint32_t i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5f;
	y = number;
	i = *(uint32_t*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	return y;
}

float pkmn_sqrt(float number) {
	return number * pkmn_inverse_sqrt(number);
}

uint32_t pkmn_pow(uint32_t base, uint32_t power) {
    if (base == 0 ) {
        return 0;
    }
    if (power == 0) {
        return 1;
    }

    uint32_t r = base;
    for (uint32_t i = 1; i < power; i++ ) {
        r *= base;
    }
    return r;
}

