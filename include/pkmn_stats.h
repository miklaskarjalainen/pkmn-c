#ifndef PKMN_STATS_H
#define PKMN_STATS_H

#include <stdint.h>

typedef struct {
	uint16_t hp, atk, def, spatk, spdef, speed;
} pkmn_stats_t;

#define PKMN_STATS(_hp, _atk, _def, _spatk, _spdef, _spd) {.hp = _hp, .atk = _atk, .def = _def, .spatk = _spatk, .spdef = _spdef, .speed = _spd}

struct pkmn_battler_t;

pkmn_stats_t pkmn_generate_empty_stats(void);
pkmn_stats_t pkmn_generate_iv_stats(void);

uint16_t pkmn_calc_other_stat(uint8_t level, uint8_t base, uint8_t iv, uint8_t ev);
uint16_t pkmn_calc_hp_stat(uint8_t level, uint8_t base, uint8_t iv, uint8_t ev);
pkmn_stats_t pkmn_calc_stats(uint8_t level, const pkmn_stats_t* base, const pkmn_stats_t* iv, const pkmn_stats_t* ev);

#endif
