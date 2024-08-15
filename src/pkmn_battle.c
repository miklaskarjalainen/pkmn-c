#include "pkmn_battle.h"
#include "pkmn_battler.h"
#include "pkmn_config.h"
#include "pkmn_rand.h"
#include "pkmn_species.h"
#include "pkmn_stats.h"
#include "pkmn_math.h"

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

void pkmn_battle_switch(
	pkmn_battler_t** user,
	pkmn_party_t* party,
	uint8_t party_index
) {
	*user = &party->battlers[party_index];
}

void pkmn_battle_move(
	pkmn_battle_t* battle,
	pkmn_battler_t* attacker,
    pkmn_battler_t* defender,
	uint8_t move_index
) {
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
	
	if (dmg.damage_done > defender->current_hp) {
		defender->current_hp = 0;
	}
	else {
		defender->current_hp -= dmg.damage_done;
	}

    battle->turn_data.details[battle->turn_data._current_half_turn].user = attacker;
    battle->turn_data.details[battle->turn_data._current_half_turn].target = defender;
    battle->turn_data.details[battle->turn_data._current_half_turn].damage = dmg;
}


void pkmn_battle_do_action(
	pkmn_battle_t* battle,
	pkmn_battle_action_t action,
	struct pkmn_battler_t** user,
	struct pkmn_party_t* user_party,
	struct pkmn_battler_t* target
) {
	if ((*user)->current_hp == 0) {
		return;
	}

	battle->turn_data.details[battle->turn_data._current_half_turn].action = action;

	switch (action.type) {
		case ACTION_MOVE: {
			pkmn_battle_move(battle, *user, target, action.move_index);
			break;
		}
		case ACTION_SWITCH: {
			pkmn_battle_switch(user, user_party, action.switch_index);
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
		(AllyIsFaster && !_pkmn_action_is_priority(opp_action)) ||
		(AllyIsFaster && _pkmn_action_is_priority(ally_action) && _pkmn_action_is_priority(opp_action)) ||
		(_pkmn_action_is_priority(ally_action) && !_pkmn_action_is_priority(opp_action))
	) {
		pkmn_battle_do_action(
			battle, ally_action, &battle->ally_active, battle->ally_party,battle->opp_active
		);
		pkmn_battle_do_action(
			battle, opp_action, &battle->opp_active, battle->opp_party, battle->ally_active
		);
	} else {
		pkmn_battle_do_action(
			battle, opp_action, &battle->opp_active, battle->opp_party, battle->ally_active
		);
		pkmn_battle_do_action(
			battle, ally_action, &battle->ally_active, battle->ally_party,battle->opp_active
		);
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

float pkmn_battler_catch_rate(const pkmn_battler_t* battler, uint16_t ball_catch_rate) {
	const uint16_t MaxHp = pkmn_battler_get_stats(battler).hp;
	const uint16_t CurHp = battler->current_hp;
	const uint8_t CatchRate = battler->species->catch_rate;
	const float BallMultiplier = ball_catch_rate / 4096.0f;
	const float A = ((3 * MaxHp - 2 * CurHp) / (3.0f * MaxHp)) * CatchRate * BallMultiplier;
	return A;
}

int pkmn_ball_shakes(const pkmn_battler_t* battler, uint16_t ball_catch_rate) {
	// https://bulbapedia.bulbagarden.net/wiki/Catch_rate#Capture_method_(Generation_III-IV)
	const float A = pkmn_battler_catch_rate(battler, ball_catch_rate);
	const uint16_t B = pkmn_sqrt(pkmn_sqrt(A/1044480.0f)) * 65535;
	
	for (int i = 0; i < 4; i++) {
		const uint16_t RandNum = pkmn_rand_u16();
		if (RandNum >= B) {
			return i;
		}
	}
	return 4;
}

