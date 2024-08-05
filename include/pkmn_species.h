#ifndef PKMN_SPECIES_H
#define PKMN_SPECIES_H

#include "pkmn_config.h"
#include "pkmn_stats.h"
#include "pkmn_type.h"

#define MAX_LEVEL_UP_MOVES 40 

struct pkmn_move_semantics_t;

typedef struct pkmn_species_t {
	char name[PKMN_MAX_NAME_LENGTH];
	pkmn_type_t types[2];
	pkmn_stats_t base, yield;
	
    struct {
        const struct pkmn_move_semantics_t* move;
        uint8_t level;
    } level_up_moves[MAX_LEVEL_UP_MOVES];
} pkmn_species_t;

#endif
