#include <catch2/catch.hpp>

extern "C" {
    #include "pkmn_battle.h"
    #include "pkmn_battler.h"
    #include "pkmn_species.h"
    #include "pkmn_rand.h"
    
    // FIXME: Make tests not require PBS files. Make Fakemons for this porpuse.
    #include "data/pkmn_pokemon_data.h"
}

#define DEMO_TEAM_1(level)       \
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

#define DEMO_TEAM_2(level)       \
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
    /*
	pkmn_init_seed(0x69420);

    pkmn_party_t party = DEMO_TEAM_1(50);
    pkmn_party_t enemies = DEMO_TEAM_2(5);

    pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
    pkmn_battle_turn(&battle,
        PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_ALLY, PKMN_BATTLE_OPPONENT, 0),
        PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0)
    );
    pkmn_battle_turn_data_t turn = battle.turn_data;

    // 
    REQUIRE(turn.actions[0].type == ACTION_MOVE);
    REQUIRE(*turn.actions[0].move_action.source_pkmn == &party.battlers[0]);

    // Second move didn't happen, because the pokemon got fainted.
    REQUIRE((*turn.actions[0].move_action.target_pkmn)->current_hp == 0);
    REQUIRE(turn.actions[1].type == ACTION_NULL);
    */
}

TEST_CASE("Battle: Speed priority", "[pkmn]") {
    /*
    pkmn_init_seed(0x69420);

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);

        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_ALLY, PKMN_BATTLE_OPPONENT, 0),
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_MOVE);
        REQUIRE(turn.actions[0].move_action.source_pkmn == 0);
    }

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);

        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_ALLY, PKMN_BATTLE_OPPONENT, 0),
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_MOVE);
        REQUIRE(turn.actions[0].move_action.source_pkmn == 0);
    }
    */
}


TEST_CASE("Battle: switch priority", "[pkmn]") {
	pkmn_init_seed(0x69420);
    /*
    
    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_SWITCH(&battle, PKMN_BATTLE_ALLY, 0, 5),
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0),
            PKMN_ACTION_SWITCH(&battle, PKMN_BATTLE_ALLY, 0, 5)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_SWITCH(&battle, PKMN_BATTLE_ALLY, 0, 5),
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);;
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(&battle, PKMN_BATTLE_OPPONENT, PKMN_BATTLE_ALLY, 0),
            PKMN_ACTION_SWITCH(&battle, PKMN_BATTLE_ALLY, 0, 5)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    */
}

TEST_CASE("Battle: Switching in", "[pkmn]") {
	pkmn_init_seed(0x69420);

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);

        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        int fainted = pkmn_battle_turn(&battle,
             {
            .type = ACTION_MOVE, .priority = 2, .speed = pkmn_battler_get_stats((&battle)->active_pkmn[0]).speed, .move_action = { .source_pkmn = 0, .target_pkmn = 1, .move_idx = 0, }
        },
            {
            .type = ACTION_MOVE, .priority = 2, .speed = pkmn_battler_get_stats((&battle)->active_pkmn[1]).speed, .move_action = { .source_pkmn = 1, .target_pkmn = 0, .move_idx = 0, }
        }
        );

        if (fainted) {
            uint8_t fainted_idx = fainted - 1;
            pkmn_battle_switch_after_faint(
                &battle, 
                {
                .type = ACTION_SWITCH, .priority = 1, .speed = pkmn_battler_get_stats((&battle)->active_pkmn[fainted_idx]).speed, .switch_action = { .field_idx = fainted_idx, .player_idx = fainted_idx, .party_idx = 4, }
            }
            );
        }

        pkmn_battle_turn_data_t turn = battle.turn_data;


        REQUIRE(fainted == 2);
        REQUIRE(turn.actions[0].type == ACTION_MOVE);
        REQUIRE(turn.actions[1].type == ACTION_SWITCH);
    }
}

