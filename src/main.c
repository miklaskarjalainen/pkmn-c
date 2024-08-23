#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "pkmn_config.h"
#include "pkmn_move.h"
#include "pkmn_stats.h"
#include "pkmn_type.h"
#include "pkmn_species.h"
#include "pkmn_battler.h"
#include "pkmn_battle.h"
#include "pkmn_battle_cli.h"
#include "pkmn_rand.h"

#include "data/pkmn_move_data.h"
#include "data/pkmn_pokemon_data.h"


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


int main(void) {
	uint32_t seed = time(NULL);
	pkmn_init_seed(seed);

	pkmn_party_t party = {
		.battlers = {
			pkmn_generate_battler(PIKACHU, 50),
			pkmn_generate_battler(BLASTOISE, 50),
			pkmn_generate_battler(VENUSAUR, 50),
			pkmn_generate_battler(CHARIZARD, 50),
			pkmn_generate_battler(ALAKAZAM, 50),
			pkmn_generate_battler(PIDGEOT, 50),
		}
	};

	pkmn_party_t enemies = {
		.battlers = {
			pkmn_generate_battler(DUSCLOPS, 50),
			pkmn_generate_battler(LUDICOLO, 50),
			pkmn_generate_battler(SKARMORY, 50),
			pkmn_generate_battler(ARMALDO, 50),
			pkmn_generate_battler(LATIOS, 50),
			pkmn_generate_battler(RAYQUAZA, 50),
		}
	};

	pkmn_battle_t battle = pkmn_battle_init(&party, &enemies);
	pkmn_cli_battle(&battle);

    return 0;
}
