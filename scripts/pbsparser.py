
allowed_chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|} "

# Parses pokemon essentials' PBS files.
class PBSParser:
    contents: str
    parsed_contents: dict

    def is_float(element: any) -> bool:
        #If you expect None to be passed:
        if element is None: 
            return False
        try:
            float(element)
            return True
        except ValueError:
            return False

    def __init__(self, path):
        self.parsed_contents = {}
        try:
            _file = open(path, "r")
            self.contents = _file.read()
            _file.close()
            print("Parser: File read!")
        except:
            print(f"ERROR: Could not read file at '{path}'")
            exit(-1)

    def parse(self):
        print("Parser: Parsing started!")

        _lines: str = self.contents.split("\n")
        _current_key: str = ""

        for line in _lines:
            line = ''.join(filter(lambda x: x in allowed_chars, line))
            # comment
            if len(line) == 0 or line.startswith("#"):
                continue
            
            # new key
            if line.startswith("["):
                _bracket_end = line.find("]")
                assert(_bracket_end != -1)
                _current_key = line[1:_bracket_end].strip()
                self.parsed_contents[_current_key] = {}
                continue

            _splitted = line.split("=")
            if len(_splitted) < 2:
                print(line)

            _field = _splitted[0].strip()
            _value = _splitted[1].strip()

            self.parsed_contents[_current_key][_field] = _value

        return self.parsed_contents

    def str_as_array(line: str) -> list[str]:
        # "1,1,1,1" -> ["1","1","1","1"]
        return line.split(",")

# Way to dump these into json
#
# parser = PBSParser("C:/Users/giffi/Downloads/Pokemon Essentials v21.1 2023-07-30/PBS/pokemon.txt")
# parsed = parser.parse()
#
# import json
# file = open("output.json", "w")
# json.dump(parser.parsed_contents, file, indent = 4)
# file.close()
