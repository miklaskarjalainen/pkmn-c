#ifndef PKMN_TYPE_H
#define PKMN_TYPE_H

#include <stdint.h>
#include <stdbool.h>

struct pkmn_battler_t;

typedef enum pkmn_type_t {
	TYPE_NULL = 0,
	TYPE_NORMAL,
	TYPE_FIGHTING,
	TYPE_FLYING,
	TYPE_POISON,
	TYPE_GROUND,
	TYPE_ROCK,
	TYPE_BUG,
	TYPE_GHOST,
	TYPE_STEEL,
	TYPE_FIRE,
	TYPE_WATER,
	TYPE_GRASS,
	TYPE_ELECTRIC,
	TYPE_PSYCHIC,
	TYPE_ICE,
	TYPE_DRAGON,
	TYPE_DARK,
	TYPE_FAIRY,
    TYPE_COUNT,
} pkmn_type_t;

typedef enum pkmn_effectiveness_t {
	IMMUNE_EFFECTIVE 		= 0,	// x0
	NOT_VERY_VERY_EFFECTIVE = 1,	// x0.25
	NOT_VERY_EFFECTIVE 	  = 2,		// x0.5
	NORMAL_EFFECTIVE  	  = 3, 		// x1.0
	SUPER_EFFECTIVE   	  = 4,		// x2.0
	SUPER_SUPER_EFFECTIVE = 5,		// x4.0
} pkmn_effectiveness_t;

typedef enum pkmn_growth_rate_t {
	EXP_FAST = 0,
	EXP_MEDIUM_FAST,
	EXP_SLOW,
	EXP_MEDIUM_SLOW,
	EXP_ERRATIC,
	EXP_FLUCTUATING,
} pkmn_growth_rate_t;

const char* pkmn_type_to_str(pkmn_type_t type);
uint32_t pkmn_xp_total_at_level(pkmn_growth_rate_t rate, uint8_t current_level);
uint32_t pkmn_xp_needed_to_level_up(pkmn_growth_rate_t rate, uint8_t current_level);
uint32_t pkmn_xp_gained_raw(
	uint16_t base_xp_yield,
	uint8_t fainted_level,
	bool trainer_pokemon,
	bool outsider_pokemon,
	uint8_t participants,
	uint8_t exp_share_count,
	bool current_has_xp_share,
	bool current_has_lucky_egg
);


bool pkmn_type_immune_to(pkmn_type_t defender, pkmn_type_t attack_type);
bool pkmn_type_weak_to(pkmn_type_t defender, pkmn_type_t attack_type);
bool pkmn_type_resistant_to(pkmn_type_t defender, pkmn_type_t attack_type);
pkmn_effectiveness_t pkmn_move_effectiveness(
	const struct pkmn_battler_t* battler,
	pkmn_type_t attack_type
);

#endif
