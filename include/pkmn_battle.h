#ifndef PKMN_BATTLE_H
#define PKMN_BATTLE_H

#include <stdint.h>
#include <stdbool.h>

struct pkmn_battler_t;
struct pkmn_party_t;
struct pkmn_move_semantics_t;
struct pkmn_move_t;

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
	ACTION_SWITCH
} pkmn_battle_action_type;

typedef struct pkmn_battle_move_action_t {
	struct pkmn_battler_t *source_pkmn, *target_pkmn;
	struct pkmn_move_t* move;
} pkmn_battle_move_action_t;

typedef struct pkmn_battle_switch_action_t {
	struct pkmn_battler_t **source_pkmn, *target_pkmn;
} pkmn_battle_switch_action_t;

typedef struct pkmn_battle_action_t {
	pkmn_battle_action_type type;
	uint8_t priority; // action priority (lower goes first)
	uint16_t speed;
	union {
		pkmn_battle_move_action_t move_action;
		pkmn_battle_switch_action_t switch_action;
	};
} pkmn_battle_action_t;

#define PKMN_ACTION_SWITCH(from, to) 				\
(pkmn_battle_action_t) {							\
	.type = ACTION_SWITCH, 							\
	.priority = 1,									\
	.speed = pkmn_battler_get_stats(from).speed,	\
	.switch_action = {								\
		.source_pkmn = &from,						\
		.target_pkmn = to,							\
	}												\
}

#define PKMN_ACTION_MOVE(src, target, move_idx)		\
(pkmn_battle_action_t) {							\
	.type = ACTION_MOVE, 							\
	.priority = 2,									\
	.speed = pkmn_battler_get_stats(src).speed,		\
	.move_action = {								\
		.source_pkmn = src,							\
		.target_pkmn = target,						\
		.move = &src->moves[move_idx],				\
	}												\
}

typedef enum pkmn_battle_damage_cause_type {
	DAMAGE_CAUSE_NIL = 0,
	DAMAGE_CAUSE_MOVE,
	DAMAGE_CAUSE_WEATHER,

	DAMAGE_CAUSE_CONFUSION,
	DAMAGE_CAUSE_STATUS,
	DAMAGE_CAUSE_CURSE,
	DAMAGE_CAUSE_COUNT,
} pkmn_battle_damage_cause_type;

typedef struct pkmn_damage_cause_t {
	pkmn_battle_damage_cause_type type;
	pkmn_damage_t damage;
} pkmn_damage_cause_t;

typedef struct pkmn_battle_turn_data_t {
	uint32_t seed;
	pkmn_battle_action_t actions[2];
	struct pkmn_damage_cause_t turn_damages[32];
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
    pkmn_battle_action_t action
);

void pkmn_battle_switch(
    pkmn_battle_t* battle,
	pkmn_battle_switch_action_t action
);

void pkmn_battle_move(
    pkmn_battle_t* battle,
	pkmn_battle_move_action_t action
);

// How many times the ball shakes after trying to capture.
// 4 Shakes means a capture
int pkmn_ball_shakes(
	const struct pkmn_battler_t* battler,
	uint16_t ball_catch_rate
);

float pkmn_battler_catch_rate(const struct pkmn_battler_t* battler, uint16_t ball_catch_rate);


#endif
