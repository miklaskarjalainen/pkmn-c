#include <catch2/catch.hpp>

extern "C" {
    #include "pkmn_battler.h"
}

TEST_CASE("Battler: shininess", "[pkmn]") {
    REQUIRE(pkmn_generated_is_shiny(0xE72DE53F, 5667, 5175) == true);
    REQUIRE(pkmn_generated_is_shiny(0x12345678, 6969, 4200) == false);
}
