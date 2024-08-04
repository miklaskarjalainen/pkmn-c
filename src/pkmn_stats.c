#include "pkmn_stats.h"
#include "pkmn_battler.h"
#include "pkmn_species.h"
#include "pkmn_rand.h"

pkmn_stats_t pkmn_generate_empty_stats(void) {
    return (pkmn_stats_t)PKMN_STATS(0,0,0,0,0,0);
}

pkmn_stats_t pkmn_generate_iv_stats(void) {
    return (pkmn_stats_t)PKMN_STATS(
        pkmn_rand_u8() % 32,
        pkmn_rand_u8() % 32,
        pkmn_rand_u8() % 32,
        pkmn_rand_u8() % 32,
        pkmn_rand_u8() % 32,
        pkmn_rand_u8() % 32
    );
}

// https://bulbapedia.bulbagarden.net/wiki/Stat
// Using the gen 3 onward algorithm
static uint16_t _pkmn_calc_other_stat(uint8_t level, uint8_t base, uint8_t iv, uint8_t ev) {
    return (((2 * base + iv + (ev / 4)) * level) / 100) + 5;
}

static uint16_t _pkmn_calc_hp_stat(uint8_t level, uint8_t base, uint8_t iv, uint8_t ev) {
    return (((2 * base + iv + (ev / 4)) * level) / 100) + level + 10;
}

pkmn_stats_t pkmn_calculate_stats(uint8_t level, const pkmn_stats_t* base, const pkmn_stats_t* iv, const pkmn_stats_t* ev) {
    return (pkmn_stats_t)
        PKMN_STATS(
            _pkmn_calc_hp_stat   (level, base->hp   , iv->hp   , ev->hp),
            _pkmn_calc_other_stat(level, base->atk  , iv->atk  , ev->atk),
            _pkmn_calc_other_stat(level, base->def  , iv->def  , ev->def),
            _pkmn_calc_other_stat(level, base->spatk, iv->spatk, ev->spatk),
            _pkmn_calc_other_stat(level, base->spdef, iv->spdef, ev->spdef),
            _pkmn_calc_other_stat(level, base->speed, iv->speed, ev->speed)
    );
}

pkmn_stats_t pkmn_calculate_stats_battler(const struct pkmn_battler_t* battler) {
    return pkmn_calculate_stats(battler->level, &battler->species->base, &battler->iv, &battler->ev);
}
