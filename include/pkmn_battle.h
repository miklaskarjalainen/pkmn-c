#ifndef PKMN_BATTLE_H
#define PKMN_BATTLE_H

#include <stdint.h>
#include <stdbool.h>

#include "pkmn_coroutine.h"


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

/*
typedef enum pkmn_battle_target {
	TARGET_SELF 	= 0,
	TARGET_OPPONENT = 1,
} pkmn_battle_target;
*/

typedef struct pkmn_battle_move_action_t {
	uint8_t source_pkmn, target_pkmn, move_idx;
} pkmn_battle_move_action_t;

typedef struct pkmn_battle_switch_action_t {
	uint8_t field_idx, player_idx, party_idx;
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

#define PKMN_BATTLE_ALLY 0
#define PKMN_BATTLE_OPPONENT 1

#define PKMN_ACTION_SWITCH(battle_ptr, active_idx_, player_idx_, party_idx_) \
(pkmn_battle_action_t) {							\
	.type = ACTION_SWITCH, 							\
	.priority = 1,									\
	.speed = pkmn_battler_get_stats(				\
		(battle_ptr)->active_pkmn[active_idx_]		\
	).speed,										\
	.switch_action = {								\
		.field_idx = active_idx_,					\
		.player_idx = player_idx_,					\
		.party_idx = party_idx_,					\
	}												\
}

#define PKMN_ACTION_MOVE(battle_ptr, active_idx, target_idx, move_idx_)	\
(pkmn_battle_action_t) {							\
	.type = ACTION_MOVE, 							\
	.priority = 2,									\
	.speed = pkmn_battler_get_stats(				\
		(battle_ptr)->active_pkmn[active_idx]		\
	).speed,										\
	.move_action = {								\
		.source_pkmn = active_idx,					\
		.target_pkmn = target_idx,					\
		.move_idx = move_idx_,						\
	}												\
}

typedef enum pkmn_battle_event_type {
	TURN_EVENT_NIL = 0,
	TURN_EVENT_SWITCH,

	TURN_EVENT_DMG_MOVE,
	TURN_EVENT_DMG_STRUGGLE,
	TURN_EVENT_DMG_WEATHER,
	TURN_EVENT_DMG_CONFUSION,
	TURN_EVENT_DMG_STATUS,
	TURN_EVENT_DMG_CURSE,
	TURN_EVENT_COUNT,
} pkmn_battle_event_type;

typedef struct pkmn_battle_event_t {
	pkmn_battle_event_type type;
	union {
		pkmn_battle_move_action_t move;
		pkmn_battle_switch_action_t switched;
	};
	pkmn_damage_t damage;
} pkmn_battle_event_t;

typedef struct pkmn_battle_turn_data_t {
	uint32_t seed;
	pkmn_battle_action_t actions[2];
	pkmn_battle_event_t events[32];
} pkmn_battle_turn_data_t;

/*
    Battle
*/

typedef struct pkmn_field_t {
	void* data;
} pkmn_field_t;

typedef struct pkmn_battle_t {
	pkmn_field_t field;

	struct pkmn_party_t* players[2];
	struct pkmn_battler_t* active_pkmn[2];

	pkmn_battle_turn_data_t turn_data;

	int fainted_idx, event_count;
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

/*
	returns true if a pokemon was fainted in the middle of the turn.
	If so then "pkmn_battle_switch_after_faint" has to be called.
*/
int pkmn_battle_turn(
    pkmn_battle_t* battle,
    pkmn_battle_action_t ally_action,
    pkmn_battle_action_t opp_action
);

/* called if pkmn_battle_turn returned true */
bool pkmn_battle_switch_after_faint(
	pkmn_battle_t* battle,
	pkmn_battle_action_t ally_action
);

/*
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
*/

// How many times the ball shakes after trying to capture.
// 4 Shakes means a capture
int pkmn_ball_shakes(
	const struct pkmn_battler_t* battler,
	uint16_t ball_catch_rate
);

float pkmn_battler_catch_rate(const struct pkmn_battler_t* battler, uint16_t ball_catch_rate);


#endif
