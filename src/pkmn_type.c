#include "pkmn_type.h"
#include "data/pkmn_pokemon_data.h"
#include "pkmn_config.h"

const char* pkmn_type_to_str(pkmn_type_t type) {
	static const char* sTypesStr[] = {
		[TYPE_NULL] = "null",
		[TYPE_NORMAL] = "normal",
		[TYPE_FIGHTING] = "fighting",
		[TYPE_FLYING] = "flying",
		[TYPE_POISON] = "poison",
		[TYPE_GROUND] = "ground",
		[TYPE_ROCK] = "rock",
		[TYPE_BUG] = "bug",
		[TYPE_GHOST] = "ghost",
		[TYPE_STEEL] = "steel",
		[TYPE_FIRE] = "fire",
		[TYPE_WATER] = "water",
		[TYPE_GRASS] = "grass",
		[TYPE_ELECTRIC] = "electric",
		[TYPE_PSYCHIC] = "psychic",
		[TYPE_ICE] = "ice",
		[TYPE_DRAGON] = "dragon",
		[TYPE_DARK] = "dark",
		[TYPE_FAIRY] = "fairy",
		[TYPE_COUNT] = "TYPE_COUNT",
	};

	return sTypesStr[type];
}

static uint32_t _pkmn_pow(uint32_t base, uint32_t power) {
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

static uint32_t _pkmn_grow_fast(uint8_t current_level) {
    return (4 * _pkmn_pow(current_level, 3)) / 5;
}

static uint32_t _pkmn_grow_medium_fast(uint8_t current_level) {
    return _pkmn_pow(current_level, 3);
}

static uint32_t _pkmn_grow_medium_slow(uint8_t current_level) {
    const float Div = (6.0f/5.0f);
    const uint32_t LevelPow = _pkmn_pow(current_level, 3);
    const uint32_t Subtract = 15 * (current_level * current_level);
    const uint32_t Addition = 100 * current_level - 140;
    return Div * LevelPow - Subtract + Addition; 
}

static uint32_t _pkmn_grow_slow(uint8_t current_level) {
    return (5 * _pkmn_pow(current_level, 3)) / 4;
}

static uint32_t _pkmn_grow_erratic(uint8_t l) {
    const uint32_t Powered = _pkmn_pow(l, 3);

    if (l < 50) {
        return (Powered * (100-l)) / 50;
    }
    if (l < 68) {
        return (Powered * (150-l)) / 100;
    }
    if (l < 98) {
        return (Powered * ((1911 - 10 * l) / 3)) / 500;
    }
    return (Powered * (160 - l)) / 100;
}

static uint32_t _pkmn_grow_fluctuating(uint8_t l) {
    const uint32_t Powered = _pkmn_pow(l, 3);
    if (l < 15) {
        return (Powered * (((l + 1) / 3) + 24)) / 50;
    }
    if (l < 36) {
        return (Powered * (l + 14)) / 50;
    }
    return (Powered * ((l / 2) + 32)) / 50;
}

uint32_t pkmn_xp_total_at_level(pkmn_growth_rate_t rate, uint8_t current_level) {
    if (current_level == 1) {
        return 0;
    }

    switch (rate) {
        case EXP_FAST: {
            return _pkmn_grow_fast(current_level);
        }
        case EXP_MEDIUM_FAST: {
            return _pkmn_grow_medium_fast(current_level);
        }
        case EXP_MEDIUM_SLOW: {
            return _pkmn_grow_medium_slow(current_level);
        }
        case EXP_SLOW: {
            return _pkmn_grow_slow(current_level);
        }
        case EXP_ERRATIC: {
            return _pkmn_grow_erratic(current_level);
        }
        case EXP_FLUCTUATING: {
            return _pkmn_grow_fluctuating(current_level);
        }

        default: {
            PKMN_PANIC("Growth rate '%i' is not implemented!", rate);
            break;
        }
    }
    return 0;
}

uint32_t pkmn_xp_needed_to_level_up(pkmn_growth_rate_t rate, uint8_t current_level) {
    return pkmn_xp_total_at_level(rate, current_level + 1) - pkmn_xp_total_at_level(rate, current_level);
}
