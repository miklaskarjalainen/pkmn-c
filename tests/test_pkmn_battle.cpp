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
    pkmn_battle_turn(&battle,
        PKMN_ACTION_MOVE(battle.ally_active, battle.opp_active, 0),
        PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0)
    );
    pkmn_battle_turn_data_t turn = battle.turn_data;

    // 
    REQUIRE(turn.actions[0].type == ACTION_MOVE);
    REQUIRE(turn.actions[0].move_action.source_pkmn == &party.battlers[0]);

    // Second move didn't happen, because the pokemon got fainted.
    REQUIRE(turn.actions[0].move_action.target_pkmn->current_hp == 0);
    REQUIRE(turn.actions[1].type == ACTION_NULL);
}

TEST_CASE("Battle: Speed priority", "[pkmn]") {
	pkmn_init_seed(0x69420);

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);

        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(battle.ally_active, battle.opp_active, 0),
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_MOVE);
        REQUIRE(turn.actions[0].move_action.source_pkmn == &party.battlers[0]);
    }

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);

        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(battle.ally_active, battle.opp_active, 0),
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_MOVE);
        REQUIRE(turn.actions[0].move_action.source_pkmn == &party.battlers[0]);
    }
}


TEST_CASE("Battle: switch priority", "[pkmn]") {
	pkmn_init_seed(0x69420);
    
    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_SWITCH(battle.ally_active, &battle.ally_party->battlers[5]),
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_1(50);
        pkmn_party_t enemies = DEMO_TEAM_2(5);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0),
            PKMN_ACTION_SWITCH(battle.ally_active, &battle.ally_party->battlers[5])
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;

        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_SWITCH(battle.ally_active, &battle.ally_party->battlers[5]),
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0)
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

    {
        pkmn_party_t party = DEMO_TEAM_2(5);;
        pkmn_party_t enemies = DEMO_TEAM_1(50);
        pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
        pkmn_battle_turn(&battle,
            PKMN_ACTION_MOVE(battle.opp_active, battle.ally_active, 0),
            PKMN_ACTION_SWITCH(battle.ally_active, &battle.ally_party->battlers[5])
        );
        pkmn_battle_turn_data_t turn = battle.turn_data;
    
        REQUIRE(turn.actions[0].type == ACTION_SWITCH);
    }

}
