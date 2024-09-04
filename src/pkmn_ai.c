#include "pkmn_ai.h"
#include "pkmn_config.h"
#include "pkmn_rand.h"
#include "pkmn_battler.h"
#include "pkmn_species.h"

pkmn_battle_action_t pkmn_ai_get_action(const struct pkmn_battle_t* battle, const pkmn_ai_t* ai, uint8_t field_idx) {
    UNUSED(ai);

    uint8_t selected_move = 0; // defaults to first move 

    const int MAX_COUNT = 32;
    for (int count = 0; count < MAX_COUNT; count++) {
        uint8_t rand = pkmn_rand_u8() % 4;
        if (battle->active_pkmn[field_idx]->moves[rand].move) {
            selected_move = rand;
        }
    }

    const uint8_t OpponentIdx = (field_idx+1) % 1;
    return PKMN_ACTION_MOVE(battle, field_idx, OpponentIdx, selected_move);
}

