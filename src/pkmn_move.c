#include "pkmn_move.h"
#include "pkmn_battler.h"
#include "pkmn_species.h"
#include "pkmn_rand.h"

pkmn_move_t pkmn_move_from_semantic(const pkmn_move_semantics_t* semantic) {
    if (!semantic) {
        return (pkmn_move_t) { NULL };
    }

    return (pkmn_move_t) {
        .move = semantic,
        .pp_left = semantic->base_pp,
        .pp_ups = 0,
    };
}
