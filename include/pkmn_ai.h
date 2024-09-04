#ifndef PKMN_AI_H
#define PKMN_AI_H

#include <stdint.h>
#include <stdbool.h>
#include "pkmn_battle.h"

typedef struct pkmn_ai_t {
    uint8_t aggressiveness, knowledge;

    // TODO: If true, the AI knows which moves the opponent has and which pokemon is in it's party.
    bool cheats; 
} pkmn_ai_t;

pkmn_battle_action_t pkmn_ai_get_action(const struct pkmn_battle_t* battle, const pkmn_ai_t* ai, uint8_t field_idx);

#endif
