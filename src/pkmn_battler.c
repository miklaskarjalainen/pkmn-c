#include "pkmn_battler.h"
#include "pkmn_rand.h"
#include "pkmn_species.h"
#include "pkmn_species.h"

bool pkmn_generated_is_shiny(uint32_t PID, uint16_t TID, uint16_t SID) {
    const uint16_t PID0_to_15  = PID & 0xFFFF;
    const uint16_t PID16_to_32 = (PID >> 16) & 0xFFFF;
    return (TID ^ SID ^ PID0_to_15 ^ PID16_to_32) < 8;
}

pkmn_battler_t pkmn_generate_battler(const struct pkmn_species_t* species, uint8_t level) {
    uint32_t PID = pkmn_rand_u32();
    uint16_t TID = (uint16_t)pkmn_rand_u32(); 
    uint16_t SID = (uint16_t)pkmn_rand_u32(); 
    
    
    return (pkmn_battler_t){
        .pid = PID,
        .nickname = { '\0' },
        .species = species,
        .iv = pkmn_generate_iv_stats(),
        .ev = pkmn_generate_empty_stats(),
        .is_shiny = pkmn_generated_is_shiny(PID, TID, SID),

        .level = level,
        .current_hp = 0, // TODO: get hp stat
        .moves = { 0 },
        .status = STATUS_NONE,
    };
}
