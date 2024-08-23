#include <stdio.h>

#include "pkmn_battle_cli.h"
#include "pkmn_battle.h"
#include "pkmn_battler.h"
#include "pkmn_stats.h"
#include "pkmn_species.h"

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

static void pkmn_print_field(const pkmn_battle_t* battle) {
    pkmn_stats_t ally_stats = pkmn_battler_get_stats(battle->ally_active);
    pkmn_stats_t opp_stats = pkmn_battler_get_stats(battle->opp_active);

    printf("Opp: '%s' lv.%u hp: %u/%u\n", 
        battle->opp_active->species->name, 
        battle->opp_active->level,
        battle->opp_active->current_hp,
        opp_stats.hp
    );
    printf("Ally: '%s' lv.%u hp: %u/%u\n", 
        battle->ally_active->species->name, 
        battle->ally_active->level,
        battle->ally_active->current_hp,
        ally_stats.hp
    );

}

void pkmn_cli_battle(struct pkmn_battle_t* battle) {
    printf("Battle began!\n");    

    printf("Player1 sent out '%s'\n", battle->ally_active->species->name);
    printf("Player2 sent out '%s'\n", battle->opp_active->species->name);
    pkmn_print_field(battle);

    char c = getc(stdin);
    printf("enterd: %c\n", c);
}

