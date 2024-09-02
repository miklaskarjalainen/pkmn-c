#include <stdio.h>

#include "pkmn_battle_cli.h"
#include "pkmn_battle.h"
#include "pkmn_battler.h"
#include "pkmn_move.h"
#include "pkmn_rand.h"
#include "pkmn_stats.h"
#include "pkmn_species.h"
#include "pkmn_config.h"

static void print_stats(const pkmn_stats_t* stats) {
	printf("[%u, %u, %u, %u, %u, %u]\n", stats->hp, stats->atk, stats->def, stats->spatk, stats->spdef, stats->speed);
}

static void print_species(const pkmn_species_t* species) {
	printf("\tSpecies: {\n");
	printf("\t\tname: '%s'\n", species->name);
	printf("\t\ttypes: [ '%s', '%s' ]\n", pkmn_type_to_str(species->types[0]), pkmn_type_to_str(species->types[1]));
	printf("\t\tbase: ");
	print_stats(&species->base);
    printf("\t\tyield: ");
	print_stats(&species->yield);
	printf("\t}\n");
}

static void print_battler(const pkmn_battler_t* battler) {
	pkmn_stats_t battler_stats = pkmn_battler_get_stats(battler);

	printf("Battler: {\n");
	printf("\tnickname: '%s'\n", battler->nickname);
	printf("\tpid: %08X\n", battler->pid);
	printf("\tlevel: '%u'\n", battler->level);
	printf("\tstats: ");
	print_stats(&battler_stats);
	printf("\tivs: ");
	print_stats(&battler->iv);
	printf("\tevs: ");
	print_stats(&battler->ev);
	printf(
		"\tmoves: ['%s', '%s', '%s', '%s']\n",
		battler->moves[0].move->name,
		battler->moves[1].move->name,
		battler->moves[2].move->name,
		battler->moves[3].move->name
	);
	print_species(battler->species);
	printf("}\n");
}

static void _pkmn_print_field(const pkmn_battle_t* battle) {
    pkmn_stats_t ally_stats = pkmn_battler_get_stats(battle->active_pkmn[0]);
    pkmn_stats_t opp_stats = pkmn_battler_get_stats(battle->active_pkmn[1]);

    printf("Opp: '%s' lv.%u hp: %u/%u spd: %u\n", 
        battle->active_pkmn[1]->species->name, 
        battle->active_pkmn[1]->level,
        battle->active_pkmn[1]->current_hp,
        opp_stats.hp,
        opp_stats.speed
    );
    printf("Ally: '%s' lv.%u hp: %u/%u spd: %u\n", 
        battle->active_pkmn[0]->species->name, 
        battle->active_pkmn[0]->level,
        battle->active_pkmn[0]->current_hp,
        ally_stats.hp,
        ally_stats.speed
    );

}

static void _pkmn_print_action_move(const pkmn_battle_move_action_t* action) {
    //printf("\t\t.move = '%s'\n", action->move->move->name);
    //printf("\t\t.source_pkmn = '%s'\n", (*action->source_pkmn)->species->name);
    //printf("\t\t.target_pkmn = '%s'\n", (*action->target_pkmn)->species->name);
}

static void _pkmn_print_action_switch(const pkmn_battle_switch_action_t* action) {
    //printf("\t\t.source_pkmn = '%s'\n", (*action->source_pkmn)->species->name);
    //printf("\t\t.target_pkmn = '%s'\n", action->target_pkmn->species->name);
}

static void _pkmn_print_action(const pkmn_battle_action_t* action) {
    printf("Action {\n");
    printf("\t.type = %s\n", action->type == ACTION_MOVE ? "ACTION_MOVE" : "ACTION_SWITCH");
    printf("\t.priority = %u\n", action->priority);
    printf("\t.action = {\n");
    if (action->type == ACTION_MOVE) {
        _pkmn_print_action_move(&action->move_action);
    }
    else {
        _pkmn_print_action_switch(&action->switch_action);
    }
    printf("\t}\n}\n");
}

pkmn_battle_action_t _pkmn_print_options(const pkmn_battle_t* battle) {
    printf("choose one of the the following:\n");

    printf("Move: \n");
    for (int i = 0; i < 4; i++) {
        const pkmn_move_t* Move = &battle->active_pkmn[0]->moves[i];

        if (!Move->move) {
            printf("\t %i) INVALID MOVE\n", i+1);
            continue;
        }
        printf(
            "\t %i) %s %i/%i\n", i+1,
            Move->move->name, Move->pp_left, Move->move->base_pp
        );
    }

    printf("Switch: \n");

    uint8_t other_party[5] = { 255, 255, 255, 255, 255 };

    bool passed_current = false;
    for (int i = 0; i < 6; i++) {
        const pkmn_battler_t* Battler = &battle->players[0]->battlers[i];
        
        if (Battler == battle->active_pkmn[0]) {
            passed_current = true;
            continue;
        }

        const int IDX = passed_current ? i + 4 : i + 5;
        if (!Battler->species) {
            printf("\t %i) NO-POKEMON\n", IDX);
            continue;
        }

        other_party[passed_current ? i - 1 : i] = i;

        pkmn_stats_t stats = pkmn_battler_get_stats(Battler);
        printf(
            "\t %i) %s %u/%u\n", IDX,
            Battler->species->name,
            Battler->current_hp,
            stats.hp
        );
    }

    int c = getc(stdin);

    switch(c) {
        case '1':
        case '2':
        case '3':
        case '4': {
            return PKMN_ACTION_MOVE(battle, 0, 1, c - '1');
        }

        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            return PKMN_ACTION_SWITCH(battle, 0, 0, other_party[c - '5']);
        }

        default: {
            printf("Invalid option!\n");
            return _pkmn_print_options(battle);
        }
    }

}

pkmn_battle_action_t _pkmn_ai_wild(const pkmn_battle_t* battle) {
    uint8_t idx = pkmn_rand_u8() % 4;
    PKMN_RUNTIME_ASSERT(battle->active_pkmn[1]->moves[idx].move != NULL, "move is null!");

    return PKMN_ACTION_MOVE(battle, 1, 0, idx);
}

void pkmn_cli_battle(struct pkmn_battle_t* battle) {
    printf("Battle began!\n");    

    printf("Player1 sent out '%s'\n", battle->active_pkmn[0]->species->name);
    printf("Player2 sent out '%s'\n", battle->active_pkmn[1]->species->name);
    
    _pkmn_print_field(battle);

    pkmn_battle_action_t player_action = _pkmn_print_options(battle);
    pkmn_battle_action_t opponent_action = _pkmn_ai_wild(battle);

    int fainted = pkmn_battle_turn(
        battle,
        player_action,
        opponent_action
    );
    
    pkmn_battle_turn_data_t data = battle->turn_data;
    
    pkmn_battle_event_t* ev_ptr = &data.events[0];
    while (ev_ptr->type != TURN_EVENT_NIL) {
        switch (ev_ptr->type) {
            case TURN_EVENT_DMG_STRUGGLE: {
                const pkmn_battler_t* attacker = battle->active_pkmn[ev_ptr->move.source_pkmn];

                printf(
                    "%s uses struggle!\n", attacker->species->name
                );
                break;
            }
            case TURN_EVENT_DMG_MOVE: {
                const pkmn_battler_t* attacker = battle->active_pkmn[ev_ptr->move.source_pkmn];

                printf(
                    "%s uses %s! which did damage: %u\n", 
                    attacker->species->name,
                    attacker->moves[ev_ptr->move.move_idx].move->name,
                    ev_ptr->damage.damage_done
                );
                break;
            }
            case TURN_EVENT_SWITCH: {
                const pkmn_battler_t* source = battle->active_pkmn[ev_ptr->switched.field_idx];
                const pkmn_battler_t* target = &battle->players[ev_ptr->switched.player_idx]->battlers[ev_ptr->switched.party_idx];

                printf(
                    "%s switched to %s\n",
                    source->species->name,
                    target->species->name
                );
                break;
            }

            default: {
                PKMN_PANIC("unhandled type %u", ev_ptr->type);
            }
        }
        ev_ptr++;
    }

    printf("during battle pokemon '%s' fainted!\n", battle->active_pkmn[fainted-1]->species->name);

    if (fainted) {
        switch (fainted) {
            case 1: {
                
                break;
            }
            case 2: {
                pkmn_battle_switch_after_faint(battle,
                    PKMN_ACTION_SWITCH(battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_OPPONENT, 2)
                );

                break;
            }

            default: {
                PKMN_PANIC("xd");
            }
        }


    }

    //_pkmn_print_action(&player_action);
    //_pkmn_print_action(&opponent_action);
    _pkmn_print_field(battle);

}

