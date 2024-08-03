#ifndef PKMN_TYPE_H
#define PKMN_TYPE_H

#define TYPE_MAX_IMMUNITIES  2
#define TYPE_MAX_RESISTANCES 4
#define TYPE_MAX_WEAKNESSES  4

typedef enum pkmn_type_t {
	TYPE_NULL = 0,
	TYPE_FIRE,
	TYPE_WATER,
	TYPE_GRASS,
    TYPE_COUNT,
} pkmn_type_t ;

/*
const static pkmn_type_t s_Weaknesses[TYPE_COUNT][TYPE_MAX_WEAKNESSES] = {
    [TYPE_NULL] = {},
    [TYPE_FIRE] = {

    }
};
*/

#endif
