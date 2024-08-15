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

typedef struct pkmn_battle_half_turn_data_t {
	const struct pkmn_battler_t* user;
	const struct pkmn_battler_t* target;
	pkmn_battle_action_t action;
	pkmn_damage_t damage;
} pkmn_battle_half_turn_data_t;

typedef struct pkmn_battle_turn_data_t {
	uint32_t seed;
	pkmn_battle_half_turn_data_t details[2];
    uint8_t _current_half_turn;


	// Theory behind this number, this is the most i can think of which would cause damage in a turn.
	// [ surf-like move hits 3 other pokemon in a double battle, so that's 3 different damages per move
	// pkmn1 move: 	SURF, SURF, SURF, ROCKY_HELMET, ROCKY_HELMET, ROCKY_HELMET, 
	// pkmn2 move:	SURF, SURF, SURF, ROCKY_HELMET, ROCKY_HELMET, ROCKY_HELMET, 
	// pkmn3 move:	SURF, SURF, SURF, ROCKY_HELMET, ROCKY_HELMET, ROCKY_HELMET, 
	// pkmn4 move:	SURF, SURF, SURF, ROCKY_HELMET, ROCKY_HELMET, ROCKY_HELMET, 
	// WEATHER, WEATHER, WEATHER, WEATHER, 
	// CURSE, CURSE, CURSE, CURSE,
	// TOXIC, TOXIC, TOXIC, TOXIC,
	// BURN, BURN, BURN, BURN,
	//]

} pkmn_battle_turn_data_t;

/*
    Battle
*/

typedef struct pkmn_field_t {
	void* data;
} pkmn_field_t;

typedef struct pkmn_battle_t {
	pkmn_field_t field;
	struct pkmn_party_t *ally_party, *opp_party;
	struct pkmn_battler_t *ally_active, *opp_active;

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
    struct pkmn_battler_t** user,
    struct pkmn_party_t* user_party,
    struct pkmn_battler_t* target
);

void pkmn_battle_switch(
	struct pkmn_battler_t** user,
	struct pkmn_party_t* party,
	uint8_t party_index
);

void pkmn_battle_move(
    pkmn_battle_t* battle,
	struct pkmn_battler_t* user,
    struct pkmn_battler_t* target,
    uint8_t move_index
);

// How many times the ball shakes after trying to capture.
// 4 Shakes means a capture
int pkmn_ball_shakes(
	const struct pkmn_battler_t* battler,
	uint16_t ball_catch_rate
);

float pkmn_battler_catch_rate(const struct pkmn_battler_t* battler, uint16_t ball_catch_rate);


#endif
