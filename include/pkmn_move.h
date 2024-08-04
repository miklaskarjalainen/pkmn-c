#ifndef PKMN_MOVE_H
#define PKMN_MOVE_H

#include <stdint.h>

#include "pkmn_config.h"
#include "pkmn_type.h"

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

static const pkmn_move_semantics_t PKMN_MOVES[] = {
	[0] = {
		.name = "Tackle",
		.category = CATEGORY_PHYSICAL,
		.type = TYPE_NORMAL,
		.power = 40,
		.accuracy = 95,
		.base_pp = 25,
	},
	[1] = {
		.name = "Ember",
		.category = CATEGORY_SPECIAL,
		.type = TYPE_FIRE,
		.power = 40,
		.accuracy = 100,
		.base_pp = 25,
	}
};

#define PKMN_MOVES_LENGTH PKMN_ARRAY_SIZE(PKMN_MOVES)

typedef struct pkmn_move_t {
	const pkmn_move_semantics_t* move;
	uint8_t pp_left;
	uint8_t pp_ups;
} pkmn_move_t;

#endif
