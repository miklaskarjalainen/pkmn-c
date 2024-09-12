from pbsparser import PBSParser

PATH_TO_FILE = "moves.txt"

# parses the dictionary passed in and spits it out as a C struct
class PBSMove:
    name: str = ""
    type: str = ""
    power: int = 0
    accuracy: int = 0
    base_pp: int = 0
    effect_chance: int = 100
    function_code: str = ""
    category: str = ""

    def __init__(self, move_data: dict):
        self.name = move_data["Name"]
        self.type = "TYPE_"+move_data["Type"].upper()
        if "Power" in move_data.keys():
            self.power = int(move_data["Power"])
        self.accuracy = int(move_data["Accuracy"])
        self.base_pp = int(move_data["TotalPP"])
        self.category = "CATEGORY_"+move_data["Category"].upper()

        if "EffectChance" in move_data.keys():
            self.effect_chance = int(move_data["EffectChance"])
        
        if "FunctionCode" in move_data.keys():
            self.function_code = PBSMove.get_function_code(move_data["FunctionCode"])


    def get_function_code(func_code: str) -> str:
        match func_code:
            case "ParalyzeTarget":
                return "MOVE_FUNCTION_PARALYZE_TARGET"
            case "BurnTarget":
                return "MOVE_FUNCTION_BURN_TARGET"
            case "FreezeTarget":
                return "MOVE_FUNCTION_FREEZE_TARGET"
            case "FlinchTarget":
                return "MOVE_FUNCTION_FLINCH_TARGET"
            case "TwoTurnAttack":
                return "MOVE_FUNCTION_TWO_TURN_ATTACK"

            case "None" | _:
                return "MOVE_FUNCTION_NONE"

    def to_string(self) -> str:
        return \
f"""{{
        .name = "{self.name}",
        .category = {self.category},
        .type = {self.type},
        .power = {self.power},
        .accuracy = {self.accuracy},
        .base_pp = {self.base_pp},
        
        .function_code = {self.function_code},
        .effect_chance = {self.effect_chance},
    }}"""


parsed = PBSParser(PATH_TO_FILE).parse()
_move_count = len(parsed.keys())

# Start building the header file.
_file_contents = \
f"""
#define MOVE_COUNT {_move_count}
const static pkmn_move_semantics_t MoveSemantics[MOVE_COUNT] = {{
"""

# Iterate through all the moves
for idx, move_name in enumerate(parsed.keys()):
    _move = PBSMove(parsed[move_name])
    _file_contents += f"\t{_move.to_string()},\n"
_file_contents += "\n};\n\n\n"
    
# Create macros for each move
for idx, move_name in enumerate(parsed.keys()):
    _file_contents += f"#define {move_name} (&MoveSemantics[{idx}])\n"


_whole_file = f"""
#ifndef _PKMN_MOVE_DATA_H
#define _PKMN_MOVE_DATA_H
#include "../pkmn_move.h"
/*
    THIS FILE IS GENERATED BY THE 'scripts/parse_move.py' SCRIPT!
    DO NOT MAKE MEANINGFUL CHANGES INTO THIS FILE!
*/

{_file_contents}
#endif
"""

# Write to file
file = open("../include/data/pkmn_move_data.h", "w")
file.write(_whole_file)
file.close()
