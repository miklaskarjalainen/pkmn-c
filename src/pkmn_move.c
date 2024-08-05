#include "pkmn_move.h"
#include "pkmn_battler.h"
#include "pkmn_species.h"
#include "pkmn_rand.h"

pkmn_move_t pkmn_move_from_semantic(const pkmn_move_semantics_t* semantic) {
    if (!semantic) {
        return (pkmn_move_t) { NULL };
    }

    return (pkmn_move_t) {
        .move = semantic,
        .pp_left = semantic->base_pp,
        .pp_ups = 0,
    };
}

// https://bulbapedia.bulbagarden.net/wiki/Damage
// GEN III
pkmn_damage_t pkmn_calculate_damage(
	const struct pkmn_battler_t* attacker,
	const struct pkmn_battler_t* defender,
	const pkmn_move_semantics_t* move
) {
    const float CRITICAL_CHANCE = 0.0625f;
    
    const int IsPhysical = move->category == CATEGORY_PHYSICAL;
    const uint16_t AtkStat = 
        IsPhysical ? pkmn_calculate_stats_battler(attacker).atk : pkmn_calculate_stats_battler(attacker).spatk;
    const uint16_t DefStat = 
        IsPhysical ? pkmn_calculate_stats_battler(defender).def : pkmn_calculate_stats_battler(defender).spdef;

    float dmg = ((((2.0f*attacker->level)/5.0f)+2.0f) * move->power * AtkStat/DefStat) / 50.0f;

    // TODO: Rest of the mechanics e.g weather, stab, burns, high crit ratio etc
    const bool CriticalHit = pkmn_randf() <= CRITICAL_CHANCE;
    if (CriticalHit) {
        dmg *= 2.0f;
    }

    const bool Stab = 
        attacker->species->types[0] == move->type ||
        attacker->species->types[1] == move->type;
    if (Stab) {
        dmg *= 1.5f;
    }

    return (pkmn_damage_t) {
        .damage_done = dmg,
        .is_critical = CriticalHit,
        .is_stab = Stab,
        .is_super_effective = false,
    };
}
