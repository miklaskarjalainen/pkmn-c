#include "pkmn_battler.h"
#include "pkmn_move.h"
#include "pkmn_rand.h"
#include "pkmn_species.h"
#include "pkmn_config.h"
#include "pkmn_stats.h"

static void _pkmn_species_latest_moves(
    const pkmn_species_t* species,
    const pkmn_move_semantics_t* moves[PKMN_BATTLER_MOVE_COUNT],
    uint8_t level
) {
    for (size_t i = 0; i < MAX_LEVEL_UP_MOVES; i++) {
        // Too high of a level
        if (species->level_up_moves[i].level >level) {
            break;
        }
        // doesn't exist
        if (species->level_up_moves[i].move == NULL) {
            break;
        }

        // we do learn this move
        moves[i % PKMN_BATTLER_MOVE_COUNT] = species->level_up_moves[i].move;
    }
}

static void _pkmn_battler_learn_latest_moves(
    pkmn_battler_t* battler
) {
    const pkmn_move_semantics_t* moves[PKMN_BATTLER_MOVE_COUNT] = { 0 };
    _pkmn_species_latest_moves(battler->species, moves, battler->level);

    for (size_t i = 0; i < PKMN_BATTLER_MOVE_COUNT; i++) {
        battler->moves[i] = pkmn_move_from_semantic(moves[i]);
    }
}

void pkmn_battler_heal(pkmn_battler_t* battler) {
    PKMN_RUNTIME_ASSERT(battler, "battler is null");

    battler->current_hp = pkmn_battler_get_stats(battler).hp;
    battler->status = STATUS_NONE;
    battler->is_confused = false;

    // Restore PPs
    for (size_t i = 0; i < PKMN_BATTLER_MOVE_COUNT; i++) {
        pkmn_move_t* pkmn_move = &battler->moves[i];
        if (pkmn_move->move == NULL) {
            continue;
        }

        const uint8_t BasePP = pkmn_move->move->base_pp;
        const uint8_t PPUpCount = pkmn_move->pp_ups;
        const uint8_t ExtraPP = (BasePP / 5) * PPUpCount;
        pkmn_move->pp_left = BasePP + ExtraPP;
    }
}

bool pkmn_calculate_shininess(uint32_t PID, uint16_t TID, uint16_t SID) {
    const uint16_t PID0_to_15  = PID & 0xFFFF;
    const uint16_t PID16_to_32 = (PID >> 16) & 0xFFFF;
    return (TID ^ SID ^ PID0_to_15 ^ PID16_to_32) < 8;
}

pkmn_battler_t pkmn_generate_battler(const struct pkmn_species_t* species, uint8_t level) {
    uint32_t PID = pkmn_rand_u32();
    uint16_t TID = (uint16_t)pkmn_rand_u32(); 
    uint16_t SID = (uint16_t)pkmn_rand_u32(); 
    
    pkmn_battler_t battler = {
        .pid = PID,
        .nickname = { '\0' },
        .species = species,
        .iv = pkmn_generate_iv_stats(),
        .ev = pkmn_generate_empty_stats(),
        .is_shiny = pkmn_calculate_shininess(PID, TID, SID),

        .level = level,
        .current_hp = 0,
        .moves = { 0 },

        .status = STATUS_NONE,
        .is_confused = false
    };

    _pkmn_battler_learn_latest_moves(&battler);
    pkmn_battler_heal(&battler);

    return battler;
}

pkmn_stats_t pkmn_battler_get_stats(const pkmn_battler_t* battler) {
    return pkmn_calc_stats(battler->level, &battler->species->base, &battler->iv, &battler->ev);
}
