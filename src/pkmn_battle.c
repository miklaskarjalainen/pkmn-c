#include <stdio.h>

#include "pkmn_battle.h"
#include "pkmn_battler.h"
#include "pkmn_config.h"
#include "pkmn_rand.h"
#include "pkmn_species.h"

static bool _pkmn_action_is_priority(pkmn_battle_action_t action) {
	return action.type != ACTION_MOVE;
}

pkmn_battle_t pkmn_battle_init(pkmn_party_t* ally_party, pkmn_party_t* opponent_party) {
	return (pkmn_battle_t){
		.field = { 0 },

		.ally_party = ally_party,
		.opp_party = opponent_party,

		.ally_active = &ally_party->battlers[0],
		.opp_active = &opponent_party->battlers[0],
	};
}

void pkmn_battle_switch(pkmn_battle_t* battle, uint8_t party_index, bool is_opponent) {
    if (is_opponent) {
	    battle->turn_data.details[battle->turn_data._current_half_turn].user = battle->opp_active;
		battle->opp_active = &battle->opp_party->battlers[party_index];
		return;
	}
	battle->turn_data.details[battle->turn_data._current_half_turn].user = battle->ally_active;
	battle->ally_active = &battle->ally_party->battlers[party_index];
}

void pkmn_battle_move(pkmn_battle_t* battle, uint8_t move_index, bool is_opponent) {
	pkmn_battler_t* attacker = is_opponent ? battle->opp_active  : battle->ally_active;
	pkmn_battler_t* defender = is_opponent ? battle->ally_active : battle->opp_active;
	pkmn_move_t* move = &attacker->moves[move_index];

	PKMN_DEBUG_ASSERT(attacker, "'attacker' IS NULL");
	PKMN_DEBUG_ASSERT(defender, "'defender' IS NULL");
	PKMN_DEBUG_ASSERT(move->move, "MOVE IS NULL");
	PKMN_DEBUG_ASSERT(move->pp_left, "MOVE DOESN'T HAVE PP");

	pkmn_damage_t dmg = pkmn_calculate_damage(
		attacker,
		defender,
		move->move
	);
    battle->turn_data.details[battle->turn_data._current_half_turn].user = attacker;
    battle->turn_data.details[battle->turn_data._current_half_turn].target = defender;
    battle->turn_data.details[battle->turn_data._current_half_turn].damage = dmg;
	
	if (dmg.damage_done > defender->current_hp) {
		defender->current_hp = 0;
		return;
	}
	defender->current_hp -= dmg.damage_done;
}

void pkmn_battle_do_action(pkmn_battle_t* battle, pkmn_battle_action_t action, bool is_opponent) {
    battle->turn_data.details[battle->turn_data._current_half_turn].action = action;
    
    switch (action.type) {
		case ACTION_MOVE: {
			pkmn_battle_move(battle, action.move_index, is_opponent);
			break;
		}
		case ACTION_SWITCH: {
			pkmn_battle_switch(battle, action.switch_index, is_opponent);
			break;
		}

		default: {
			PKMN_PANIC("Unimplemented action used '%i'!", (int)action.type);
			break;
		}
	}

    battle->turn_data._current_half_turn++;
}

pkmn_battle_turn_data_t pkmn_battle_turn(
    pkmn_battle_t* battle,
    pkmn_battle_action_t ally_action, 
    pkmn_battle_action_t opp_action
) {
	// Init turn data
	battle->turn_data = (pkmn_battle_turn_data_t){ 0 };
	battle->turn_data.seed = pkmn_rand_get_seed();
    battle->turn_data._current_half_turn = 0;

	// TODO: paralysis
	const pkmn_stats_t AllyStats = pkmn_battler_get_stats(battle->ally_active);
	const pkmn_stats_t OppStats = pkmn_battler_get_stats(battle->opp_active);
	const bool AllyIsFaster = 
		AllyStats.speed == OppStats.speed ?
			pkmn_randf() > 0.5 : AllyStats.speed > OppStats.speed;

	// Ally goes first 
	if (
		AllyIsFaster && 
		(!_pkmn_action_is_priority(opp_action) || _pkmn_action_is_priority(ally_action))
	) {
		pkmn_battle_do_action(battle, ally_action, false);
		pkmn_battle_do_action(battle, opp_action, true);
	} else {
		pkmn_battle_do_action(battle, opp_action, true);
		pkmn_battle_do_action(battle, ally_action, false);
	}

	return battle->turn_data;
}

pkmn_damage_t pkmn_calculate_damage(
	const struct pkmn_battler_t* attacker,
	const struct pkmn_battler_t* defender,
	const pkmn_move_semantics_t* move
) {
    const float CRITICAL_CHANCE = 0.0625f;
    
    const int IsPhysical = move->category == CATEGORY_PHYSICAL;
    const uint16_t AtkStat = 
        IsPhysical ? pkmn_battler_get_stats(attacker).atk : pkmn_battler_get_stats(attacker).spatk;
    const uint16_t DefStat = 
        IsPhysical ? pkmn_battler_get_stats(defender).def : pkmn_battler_get_stats(defender).spdef;

    float dmg = ((((2.0f*attacker->level)/5.0f)+2.0f) * move->power * AtkStat/DefStat) / 50.0f;

    // TODO: Rest of the mechanics e.g weather, stab, burns, high crit ratio etc
    const bool CriticalHit = pkmn_randf() <= 0.5f;
    if (CriticalHit) {
        dmg *= 2.0f;
    }

    const bool Stab = 
        attacker->species->types[0] == move->type ||
        attacker->species->types[1] == move->type;
    if (Stab) {
        dmg *= 1.5f;
    }
    const float Random = pkmn_rand_range(85, 100) / 100.0f;
    dmg *= Random;

    return (pkmn_damage_t) {
        .damage_done = dmg,
        .is_critical = CriticalHit,
        .is_stab = Stab,
        .is_super_effective = false,
    };
}

