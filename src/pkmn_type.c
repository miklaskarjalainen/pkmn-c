#include "pkmn_type.h"
#include "pkmn_config.h"
#include "pkmn_math.h"
#include "pkmn_battler.h"
#include "pkmn_config.h"
#include "data/pkmn_pokemon_data.h"

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

static uint32_t _pkmn_grow_fast(uint8_t current_level) {
    return (4 * pkmn_pow(current_level, 3)) / 5;
}

static uint32_t _pkmn_grow_medium_fast(uint8_t current_level) {
    return pkmn_pow(current_level, 3);
}

static uint32_t _pkmn_grow_medium_slow(uint8_t current_level) {
    const float Div = (6.0f/5.0f);
    const uint32_t LevelPow = pkmn_pow(current_level, 3);
    const uint32_t Subtract = 15 * (current_level * current_level);
    const uint32_t Addition = 100 * current_level - 140;
    return Div * LevelPow - Subtract + Addition; 
}

static uint32_t _pkmn_grow_slow(uint8_t current_level) {
    return (5 * pkmn_pow(current_level, 3)) / 4;
}

static uint32_t _pkmn_grow_erratic(uint8_t l) {
    const uint32_t Powered = pkmn_pow(l, 3);

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
    const uint32_t Powered = pkmn_pow(l, 3);
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

// https://bulbapedia.bulbagarden.net/wiki/Experience#Experience_gain_in_battle
// uses the formula for the first 1-4 gens.
uint32_t pkmn_xp_gained_raw(
    uint16_t b, uint8_t l,
    bool trainer_pokemon,
    bool outsider_pokemon,
    uint8_t participants,
    uint8_t share_count,
    bool current_share,
    bool current_lucky_egg
) {
    PKMN_RUNTIME_ASSERT(participants > 0, "participant count is 0!");

    const float A = trainer_pokemon ? 1.5f : 1.0f;
    const float E = current_lucky_egg ? 1.5f : 1.0f;
    const float T = outsider_pokemon ? 1.5f : 1.0f;
    const uint8_t S = 
        share_count ? 
            current_share ? 
                share_count * 2
            :
                participants * 2
        :
            participants;
    return ((b * l) / 7.0f) * (1.0f / (float)S) * E * A * T;
}

static bool _has_type(pkmn_type_t target, const pkmn_type_t* arr, size_t len) {
    for (size_t i = 0; i < len; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

bool pkmn_type_immune_to(pkmn_type_t defender, pkmn_type_t attack_type) {
    switch(defender) {
        case TYPE_NORMAL:
            return attack_type == TYPE_GHOST;
        case TYPE_FLYING:
            return attack_type == TYPE_GROUND;
        case TYPE_GROUND:
            return attack_type == TYPE_ELECTRIC;
        case TYPE_GHOST:
            return attack_type == TYPE_NORMAL || attack_type == TYPE_FIGHTING;
        case TYPE_STEEL:
            return attack_type == TYPE_POISON;
        case TYPE_DARK:
            return attack_type == TYPE_PSYCHIC;
        case TYPE_FAIRY:
            return attack_type == TYPE_DRAGON;
        default:
            return false;
    }
}

bool pkmn_type_weak_to(pkmn_type_t defender, pkmn_type_t attack_type) {
    switch (defender) {
        case TYPE_NORMAL: {
            return attack_type == TYPE_FIGHTING;
        }
        case TYPE_FIGHTING: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FLYING, TYPE_PSYCHIC, TYPE_FAIRY
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_FLYING: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_ROCK, TYPE_ELECTRIC, TYPE_ICE
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_POISON: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_GROUND, TYPE_PSYCHIC
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_GROUND: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_WATER, TYPE_GRASS, TYPE_ICE
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_ROCK: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FIGHTING, TYPE_GROUND, TYPE_STEEL, TYPE_WATER, TYPE_GRASS
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_BUG: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FLYING, TYPE_ROCK, TYPE_FIRE
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_GHOST: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_GHOST, TYPE_DARK
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_STEEL: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FIGHTING, TYPE_GROUND, TYPE_FIRE
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_FIRE: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_GROUND, TYPE_ROCK, TYPE_WATER
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_WATER: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_GRASS, TYPE_ELECTRIC
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_GRASS: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FLYING, TYPE_POISON, TYPE_BUG, TYPE_FIRE, TYPE_ICE
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_ELECTRIC: {
            return attack_type == TYPE_GROUND;
        }
        case TYPE_PSYCHIC: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_BUG, TYPE_GHOST, TYPE_DARK
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_DRAGON: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_ICE, TYPE_DRAGON, TYPE_FAIRY
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_DARK: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_FIGHTING, TYPE_BUG, TYPE_FAIRY
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        case TYPE_FAIRY: {
            static const pkmn_type_t WEAKNESSES[] = {
                TYPE_POISON, TYPE_STEEL
            };
            return _has_type(attack_type, WEAKNESSES, PKMN_ARRAY_SIZE(WEAKNESSES));
        }
        default: {
            PKMN_VERIFY_RETURN(false, false, "Weaknesses are not implemented for type '%u'!", defender);
        }
    }
}

bool pkmn_type_resistant_to(
    pkmn_type_t defender, 
    pkmn_type_t attack_type
) {
    switch (defender) {
        case TYPE_NORMAL: {
            return false;
        }
        case TYPE_FIGHTING: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_ROCK, TYPE_BUG, TYPE_DARK
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_FLYING: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIGHTING, TYPE_BUG, TYPE_GRASS
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_POISON: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIGHTING, TYPE_POISON, TYPE_BUG, TYPE_GRASS, TYPE_FAIRY
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_GROUND: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_POISON, TYPE_ROCK
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_ROCK: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_NORMAL, TYPE_FLYING, TYPE_POISON, TYPE_FIRE
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_BUG: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIGHTING, TYPE_GROUND, TYPE_GRASS
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_GHOST: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_POISON, TYPE_BUG
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_STEEL: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_NORMAL, TYPE_FLYING, TYPE_ROCK, TYPE_BUG, TYPE_STEEL, TYPE_GRASS, TYPE_PSYCHIC,
                TYPE_ICE, TYPE_DRAGON, TYPE_FAIRY
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_FIRE: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_BUG, TYPE_STEEL, TYPE_FIRE, TYPE_GRASS, TYPE_ICE,TYPE_FAIRY
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_WATER: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_GROUND, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_ELECTRIC: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FLYING, TYPE_STEEL, TYPE_ELECTRIC
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_PSYCHIC: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIGHTING, TYPE_PSYCHIC
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_ICE: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_ICE,
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_DRAGON: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIRE, TYPE_WATER, TYPE_GRASS, TYPE_ELECTRIC
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_DARK: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_GHOST, TYPE_DARK
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }
        case TYPE_FAIRY: {
            static const pkmn_type_t RESISTANCES[] = {
                TYPE_FIGHTING, TYPE_BUG, TYPE_DARK
            };
            return _has_type(attack_type, RESISTANCES, PKMN_ARRAY_SIZE(RESISTANCES));
        }

        default: {
            PKMN_VERIFY_RETURN(false, false, "Resistances are not implemented for type '%u'!", defender);
        }
    }
    
}

pkmn_effectiveness_t pkmn_move_effectiveness(
	const pkmn_battler_t* battler,
	pkmn_type_t attack_type
) {
    uint8_t effectiveness = (uint8_t)NORMAL_EFFECTIVE;

    for (uint8_t i = 0; i < PKMN_ARRAY_SIZE(battler->species->types); i++) {
        pkmn_type_t defend_type = battler->species->types[i];
        if (defend_type == TYPE_NULL) {
            continue;
        }
        
        if (pkmn_type_immune_to(defend_type, attack_type)) {
            effectiveness = 0;
            break;
        }
        if (pkmn_type_resistant_to(defend_type, attack_type)) {
            effectiveness -= 1;
            continue;
        }
        if (pkmn_type_weak_to(defend_type, attack_type)) {
            effectiveness += 1;
            continue;
        }
    }

    PKMN_RUNTIME_ASSERT(effectiveness <= SUPER_SUPER_EFFECTIVE, "effectiveness over supported.");
    return (pkmn_effectiveness_t)effectiveness;
}

