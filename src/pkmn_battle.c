#include "pkmn_battle.h"
#include "pkmn_battler.h"
#include "pkmn_config.h"
#include "pkmn_rand.h"
#include "pkmn_species.h"
#include "pkmn_stats.h"
#include "pkmn_math.h"

#define _BATTLE_ADD_EVENT(battle, ev_ptr, ev) (battle)->turn_data.events[(*ev_ptr)++] = ev
#define _MOVE_EVENT(move_ptr, from_ptr, to_ptr, dmg) (pkmn_battle_event_t) {.type = TURN_EVENT_DMG_MOVE, .from = from_ptr, .to = to_ptr, .damage = dmg, .move = move_ptr}
#define _DAMAGE_EVENT(dmg_cause, from_ptr, to_ptr, dmg) (pkmn_battle_event_t) {.type = dmg_cause, .from = from_ptr, .to = to_ptr, .damage = dmg, .move = NULL }
#define _SWITCH_EVENT(from_ptr, to_ptr) (pkmn_battle_event_t) {.type = TURN_EVENT_SWITCH, .from = from_ptr, .to = to_ptr }

static bool _pkmn_action_is_priority(pkmn_battle_action_t action) {
	return action.type != ACTION_MOVE;
}

static void _pkmn_sort_actions(pkmn_battle_action_t* action, size_t length) {
	for (size_t i = 0; i < length-1; i++) {
		for (size_t j = 0; j < length-i-1; j++) {
			// swap occurs when J+1 has priority
			if (action[j].priority > action[j+1].priority) {
				goto swap;
			}
			else if (action[j].priority == action[j+1].priority) {
				if (action[j].speed < action[j+1].speed) {
					goto swap;
				}
				// speed tie, swap ~50% of the time
				else if (action[j].speed == action[j+1].speed && pkmn_randf() >= 0.5) {
					goto swap;
				}
			}
			continue;

			swap:;
			pkmn_battle_action_t hold = action[j];
			action[j] = action[j+1];
			action[j+1] = hold;
		}
	}
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

static void _pkmn_battle_switch(
	pkmn_battle_t* battle,
	pkmn_battle_switch_action_t action,
	uint8_t* event_count
) {
	_BATTLE_ADD_EVENT(battle, event_count, 
		_SWITCH_EVENT(*action.source_pkmn, action.target_pkmn)
	);
	*action.source_pkmn = action.target_pkmn;
}

static void _pkmn_battle_move(
	pkmn_battle_t* battle,
	pkmn_battle_move_action_t action,
	uint8_t* event_count
) {
	if (action.source_pkmn->current_hp == 0) {
		return;
	}

	PKMN_DEBUG_ASSERT(action.source_pkmn, "'attacker' IS NULL");
	PKMN_DEBUG_ASSERT(action.target_pkmn, "'defender' IS NULL");
	PKMN_DEBUG_ASSERT(action.move, "MOVE IS NULL");
	PKMN_DEBUG_ASSERT(action.move->pp_left, "MOVE DOESN'T HAVE PP");

	action.move->pp_left--;

	pkmn_damage_t dmg = pkmn_calculate_damage(
		action.source_pkmn,
		action.target_pkmn,
		action.move->move
	);
	_BATTLE_ADD_EVENT(battle, event_count,
		_MOVE_EVENT(action.move, action.source_pkmn, action.target_pkmn, dmg)
	);
	
	if (dmg.damage_done > action.target_pkmn->current_hp) {
		action.target_pkmn->current_hp = 0;
	}
	else {
		action.target_pkmn->current_hp -= dmg.damage_done;
	}
}

static void _pkmn_battle_do_action(
	pkmn_battle_t* battle,
	pkmn_battle_action_t action,
	uint8_t* event_count
) {
	PKMN_RUNTIME_ASSERT(*event_count <= PKMN_ARRAY_SIZE(battle->turn_data.events), "turn_data.events overflowed");

	switch (action.type) {
		case ACTION_MOVE: {
			_pkmn_battle_move(battle, action.move_action, event_count);
			break;
		}
		case ACTION_SWITCH: {
			_pkmn_battle_switch(battle, action.switch_action, event_count);
			break;
		}

		default: {
			PKMN_PANIC("Unimplemented action used '%i'!", (int)action.type);
			break;
		}
	}

}

bool pkmn_battle_turn(
    pkmn_battle_t* battle,
    pkmn_battle_action_t ally_action, 
    pkmn_battle_action_t opp_action
) {
	// Init turn data
	battle->turn_data = (pkmn_battle_turn_data_t){ 0 };
	battle->turn_data.seed = pkmn_rand_get_seed();

	battle->turn_data.actions[0] = ally_action;
	battle->turn_data.actions[1] = opp_action;

	_pkmn_sort_actions(battle->turn_data.actions,PKMN_ARRAY_SIZE(battle->turn_data.actions));

	uint8_t event_count = 0;
	for (size_t action_idx = 0; action_idx < PKMN_ARRAY_SIZE(battle->turn_data.actions); action_idx++) {
		_pkmn_battle_do_action(battle, battle->turn_data.actions[action_idx], &event_count);
	}

	return false;
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

