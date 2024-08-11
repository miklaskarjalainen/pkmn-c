#include <catch2/catch.hpp>

extern "C" {
    #include "pkmn_battle.h"
    #include "pkmn_battler.h"
    #include "pkmn_species.h"
    #include "pkmn_rand.h"
    
    // FIXME: Make tests not require PBS files. Make Fakemons for this porpuse.
    #include "data/pkmn_pokemon_data.h"
}

#define DEMO_TEAM_1(level) (pkmn_party_t)       \
{                                               \
    .battlers = {                               \
        pkmn_generate_battler(PIKACHU, level),  \
        pkmn_generate_battler(BLASTOISE, level),\
        pkmn_generate_battler(VENUSAUR, level), \
        pkmn_generate_battler(CHARIZARD, level),\
        pkmn_generate_battler(ALAKAZAM, level), \
        pkmn_generate_battler(PIDGEOT, level),  \
    }                                           \
}

#define DEMO_TEAM_2(level) (pkmn_party_t)       \
{                                               \
    .battlers = {                               \
        pkmn_generate_battler(DUSCLOPS, level), \
        pkmn_generate_battler(LUDICOLO, level), \
        pkmn_generate_battler(SKARMORY, level), \
        pkmn_generate_battler(ARMALDO, level),  \
        pkmn_generate_battler(LATIOS, level),   \
        pkmn_generate_battler(RAYQUAZA, level), \
    }                                           \
}

TEST_CASE("Battle: Fainting", "[pkmn]") {
	pkmn_init_seed(0x69420);

    pkmn_party_t party = DEMO_TEAM_1(50);
    pkmn_party_t enemies = DEMO_TEAM_2(5);

    pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
    pkmn_battle_turn_data_t turn = pkmn_battle_turn(&battle,
        (pkmn_battle_action_t){
            .type = ACTION_MOVE,
            .move_index = 1
        },
        (pkmn_battle_action_t){
            .type = ACTION_MOVE,
            .move_index = 2
        }
    );

    // Second move didn't happen, because the pokemon got fainted.
    REQUIRE(turn.details[0].target->current_hp == 0);
    REQUIRE(turn.details[1].action.type == ACTION_NULL);
}

TEST_CASE("Battle: switch priority", "[pkmn]") {
	pkmn_init_seed(0x69420);

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn_data_t turn = pkmn_battle_turn(&battle,
            (pkmn_battle_action_t){
                .type = ACTION_SWITCH,
                .switch_index = 0
            },
            (pkmn_battle_action_t){
                .type = ACTION_MOVE,
                .move_index = 2
            }
        );
    
        REQUIRE(turn.details[0].action.type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn_data_t turn = pkmn_battle_turn(&battle,
            (pkmn_battle_action_t){
                .type = ACTION_MOVE,
                .move_index = 0
            },
            (pkmn_battle_action_t){
                .type = ACTION_SWITCH,
                .switch_index = 2
            }
        );
    
        REQUIRE(turn.details[0].action.type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn_data_t turn = pkmn_battle_turn(&battle,
            (pkmn_battle_action_t){
                .type = ACTION_SWITCH,
                .switch_index = 0
            },
            (pkmn_battle_action_t){
                .type = ACTION_MOVE,
                .move_index = 2
            }
        );
    
        REQUIRE(turn.details[0].action.type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);;
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn_data_t turn = pkmn_battle_turn(&battle,
            (pkmn_battle_action_t){
                .type = ACTION_MOVE,
                .move_index = 0
            },
            (pkmn_battle_action_t){
                .type = ACTION_SWITCH,
                .switch_index = 2
            }
        );
    
        REQUIRE(turn.details[0].action.type == ACTION_SWITCH);
    }
}
