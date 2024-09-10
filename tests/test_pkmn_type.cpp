#include <catch2/catch.hpp>

extern "C" {
    #include "pkmn_type.h"
    #include "pkmn_battler.h"
    #include "data/pkmn_pokemon_data.h"
}

TEST_CASE("Type: xp total", "[pkmn]") {
    // FAST
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 5) == 100);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 10) == 800);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 25) == 12500);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 47) == 83058);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 80) == 409600);
        REQUIRE(pkmn_xp_total_at_level(EXP_FAST, 100) == 800000);
    }
    
    // MEDIUM FAST
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 5) == 125);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 10) == 1000);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 25) == 15625);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 47) == 103823);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 80) == 512000);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_FAST, 100) == 1000000);
    }

    // MEDIUM SLOW
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 5) == 135);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 10) == 560);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 25) == 11735);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 47) == 96012);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 80) == 526260);
        REQUIRE(pkmn_xp_total_at_level(EXP_MEDIUM_SLOW, 100) == 1059860);
    }

    // SLOW
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 5) == 156);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 10) == 1250);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 25) == 19531);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 47) == 129778);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 80) == 640000);
        REQUIRE(pkmn_xp_total_at_level(EXP_SLOW, 100) == 1250000);
    }

    // ERRATIC
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 5) == 237);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 10) == 1800);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 25) == 23437);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 47) == 110052);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 80) == 378880);
        REQUIRE(pkmn_xp_total_at_level(EXP_ERRATIC, 100) == 600000);
    }

    // FLUCTUATING
    {
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 1) == 0);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 5) == 65);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 10) == 540);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 25) == 12187);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 47) == 114205);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 80) == 737280);
        REQUIRE(pkmn_xp_total_at_level(EXP_FLUCTUATING, 100) == 1640000);
    }
}

TEST_CASE("Type: xp next level", "[pkmn]") {
    // FAST
    {
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 1) == 6);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 5) == 72);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 10) == 264);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 25) == 1560);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 47) == 5415);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 80) == 15552);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FAST, 99) == 23761);
    }

    // ERRATIC
    {
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 1) == 15);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 5) == 169);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 10) == 569);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 25) == 2575);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 47) == 4963);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 80) == 11197);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_ERRATIC, 99) == 8118);
    }

    // FLUCTUATING
    {
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 1) == 4);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 5) == 47);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 10) == 205);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 25) == 1873);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 47) == 9658);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 80) == 27995);
        REQUIRE(pkmn_xp_needed_to_level_up(EXP_FLUCTUATING, 99) == 68116);
    }
}

TEST_CASE("Type: immunities", "[pkmn]") {
    // Immunities
    {
        REQUIRE(pkmn_type_immune_to(TYPE_FLYING, TYPE_GROUND) == true);
        REQUIRE(pkmn_type_immune_to(TYPE_GHOST, TYPE_NORMAL) == true);
        REQUIRE(pkmn_type_immune_to(TYPE_NORMAL, TYPE_GHOST) == true);

        REQUIRE(pkmn_type_immune_to(TYPE_NORMAL, TYPE_NORMAL) == false);
        REQUIRE(pkmn_type_immune_to(TYPE_FIRE, TYPE_WATER) == false);
        REQUIRE(pkmn_type_immune_to(TYPE_FIRE, TYPE_GRASS) == false);
    }
}

TEST_CASE("Type: pkmn move effectiveness", "[pkmn]") {
    // Charizard
    {
        pkmn_battler_t charizard = pkmn_generate_battler(CHARIZARD, 50);   
        REQUIRE(pkmn_move_effectiveness(&charizard, TYPE_GROUND) == IMMUNE_EFFECTIVE);
        
        REQUIRE(pkmn_move_effectiveness(&charizard, TYPE_ROCK) == SUPER_SUPER_EFFECTIVE);
        REQUIRE(pkmn_move_effectiveness(&charizard, TYPE_ELECTRIC) == SUPER_EFFECTIVE);
        REQUIRE(pkmn_move_effectiveness(&charizard, TYPE_WATER) == SUPER_EFFECTIVE);
        REQUIRE(pkmn_move_effectiveness(&charizard, TYPE_GRASS) == NOT_VERY_VERY_EFFECTIVE);
    }
}
