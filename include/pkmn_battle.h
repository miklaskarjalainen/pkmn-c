#ifndef PKMN_BATTLE_H
#define PKMN_BATTLE_H

#include <stdint.h>
#include <stdbool.h>

struct pkmn_battler_t;
struct pkmn_party_t;
struct pkmn_move_semantics_t;

/*
    Turn details
*/

typedef struct pkmn_damage_t {
	uint16_t damage_done;
	bool is_critical;
	bool is_stab;
	bool is_super_effective;
} pkmn_damage_t;

typedef enum pkmn_battle_action_type {
	ACTION_NULL = 0,
	ACTION_MOVE,
	ACTION_ITEM,
	ACTION_SWITCH,
	ACTION_RUN
} pkmn_battle_action_type;

typedef struct pkmn_battle_action_t {
	pkmn_battle_action_type type;
	union {
		uint8_t move_index;
		uint8_t switch_index;
		uint16_t item_id;
	};
} pkmn_battle_action_t;

typedef struct pkmn_battle_turn_data_t {
	uint32_t seed;
	struct {
        pkmn_battle_action_t action;
		const struct pkmn_battler_t* user;
		pkmn_damage_t damage;
	} details[2];

    uint8_t _current_half_turn;
} pkmn_battle_turn_data_t;

/*
    Battle
*/

typedef struct pkmn_field_t {
	void* data;
} pkmn_field_t;

typedef struct pkmn_battle_t {
	pkmn_field_t field;
	struct pkmn_party_t* ally_party, *opp_party;
	struct pkmn_battler_t* ally_active, *opp_active;

	pkmn_battle_turn_data_t turn_data;
} pkmn_battle_t;

// https://bulbapedia.bulbagarden.net/wiki/Damage
// GEN III
pkmn_damage_t pkmn_calculate_damage(
    const struct pkmn_battler_t* attacker,
    const struct pkmn_battler_t* defender, 
    const struct pkmn_move_semantics_t* move
);
pkmn_battle_t pkmn_battle_init(
    struct pkmn_party_t* ally_party,
    struct pkmn_party_t* opponent_party
);
pkmn_battle_turn_data_t pkmn_battle_turn(
    pkmn_battle_t* battle,
    pkmn_battle_action_t ally_action,
    pkmn_battle_action_t opp_action
);

void pkmn_battle_do_action(
    pkmn_battle_t* battle,
    pkmn_battle_action_t action,
    bool is_opponent
);
void pkmn_battle_switch(
    pkmn_battle_t* battle,
    uint8_t party_index,
    bool is_opponent
);
void pkmn_battle_move(
    pkmn_battle_t* battle,
    uint8_t move_index,
    bool is_opponent
);

#endif
