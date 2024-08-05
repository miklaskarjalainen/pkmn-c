#ifndef PKMN_MOVE_H
#define PKMN_MOVE_H

#include <stdint.h>
#include <stdbool.h>

#include "pkmn_config.h"
#include "pkmn_type.h"

struct pkmn_battler_t;

typedef enum pkmn_move_category_t {
	CATEGORY_SPECIAL,
	CATEGORY_PHYSICAL,
	CATEGORY_STATUS,
} pkmn_move_category_t;

typedef struct pkmn_move_semantics_t {
	char name[PKMN_MAX_NAME_LENGTH];
	pkmn_move_category_t category;
	pkmn_type_t type;
	uint8_t power;
	uint8_t accuracy;
	uint8_t base_pp;
} pkmn_move_semantics_t;

typedef struct pkmn_move_t {
	const pkmn_move_semantics_t* move;
	uint8_t pp_left;
	uint8_t pp_ups;
} pkmn_move_t;

typedef struct pkmn_damage_t {
	uint16_t damage_done;
	bool is_critical;
	bool is_stab;
	bool is_super_effective;
} pkmn_damage_t;


pkmn_move_t pkmn_move_from_semantic(const pkmn_move_semantics_t* semantic);
// https://bulbapedia.bulbagarden.net/wiki/Damage
// GEN III
pkmn_damage_t pkmn_calculate_damage(const struct pkmn_battler_t* attacker, const struct pkmn_battler_t* defender, const pkmn_move_semantics_t* move);


#endif
