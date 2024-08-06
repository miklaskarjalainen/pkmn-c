#ifndef PKMN_BATTLER_H
#define PKMN_BATTLER_H

#include <stdbool.h>
#include "pkmn_config.h"
#include "pkmn_stats.h"
#include "pkmn_move.h"

#define PKMN_BATTLER_MOVE_COUNT 4
#define PKMN_MAX_PARTY_SIZE 6

struct pkmn_species_t;

typedef enum pkmn_status_t {
    STATUS_NONE = 0,
    STATUS_FREEZE,
    STATUS_PARALYSIS,
    
    STATUS_POISON,
    STATUS_TOXIC,

    STATUS_SLEEP,
    STATUS_DROWSY,
} pkmn_status_t;

typedef struct pkmn_battler_t {
    uint32_t pid;
	char nickname[PKMN_MAX_NAME_LENGTH];
	const struct pkmn_species_t* species;
	pkmn_stats_t iv, ev;
    bool is_shiny;
    
    uint8_t level;
    uint8_t current_hp;
    pkmn_move_t moves[PKMN_BATTLER_MOVE_COUNT];

    pkmn_status_t status;
    bool is_confused;
} pkmn_battler_t;

typedef struct pkmn_party_t {
	pkmn_battler_t battlers[PKMN_MAX_PARTY_SIZE];
} pkmn_party_t;

// https://bulbapedia.bulbagarden.net/wiki/Shiny_Pokémon
bool pkmn_calculate_shininess(uint32_t PID, uint16_t TID, uint16_t SID);
void pkmn_battler_heal(pkmn_battler_t* battler);
pkmn_battler_t pkmn_generate_battler(const struct pkmn_species_t* species, uint8_t level);
pkmn_stats_t pkmn_battler_get_stats(const pkmn_battler_t* battler);

#endif
