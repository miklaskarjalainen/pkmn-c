#include <stdio.h>
#include <time.h>

#include "pkmn_config.h"
#include "pkmn_move.h"
#include "pkmn_stats.h"
#include "pkmn_type.h"
#include "pkmn_species.h"
#include "pkmn_battler.h"
#include "pkmn_rand.h"

void print_stats(const pkmn_stats_t* stats) {
	printf("[%u, %u, %u, %u, %u, %u]\n", stats->hp, stats->atk, stats->def, stats->spatk, stats->spdef, stats->speed);
}

void print_species(const pkmn_species_t* species) {
	printf("\tSpecies: {\n");
	printf("\t\tname: '%s'\n", species->name);
	printf("\t\ttypes: [ '%s', '%s' ]\n", pkmn_type_to_str(species->types[0]), pkmn_type_to_str(species->types[1]));
	printf("\t\tbase: ");
	print_stats(&species->base);
    printf("\t\tyield: ");
	print_stats(&species->yield);
	printf("\t}\n");
}

void print_battler(const pkmn_battler_t* battler) {
	pkmn_stats_t battler_stats = pkmn_calculate_stats_battler(battler);

	printf("Battler: {\n");
	printf("\tnickname: '%s'\n", battler->nickname);
	printf("\tpid: %x\n", battler->pid);
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

#include "pkmn_move_data.h"
#include "pkmn_pokemon_data.h"

int main(void) {
	pkmn_init_seed(time(NULL));

    const pkmn_move_semantics_t* m = BULLETSEED; 
    printf("%u\n", m->accuracy);

    pkmn_battler_t zekrom = pkmn_generate_battler(ZEKROM, 50);
    pkmn_battler_t charmander = pkmn_generate_battler(CHARMANDER, 50); 

	pkmn_damage_t dmg = pkmn_calculate_damage(&zekrom, &charmander, THUNDER);

    printf("dmg: { dmg = %u, crit = %i, stab = %i)\n", dmg.damage_done, dmg.is_critical, dmg.is_stab);
	print_battler(&zekrom);
	print_battler(&charmander);
	

    printf(
        "IS SHINY %s\n", pkmn_calculate_shininess(0xE72DE53F, 5667, 5175) ? "true" : "false"
    );

    return 0;
}
