# "Unreal Pokémon" created by Retro & Chill.
import os
import unittest

from unreal import Stat, Name, PokemonStatType, Text

from coverage_helpers import run_test_with_coverage
from pokemon.data_loader import IniData
from pokemon.data_loader.pbs_data import ItemData, SpeciesData

ROOT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))


class TestIniData(unittest.TestCase):
    def test_get_item(self):
        ini_data = IniData(os.path.join(ROOT_DIR, "resources/items.txt"))
        self.assertEqual("Repel", ini_data["REPEL"]["Name"])
        self.assertEqual("Repels", ini_data["REPEL"]["NamePlural"])
        self.assertEqual("700", ini_data["SUPERREPEL"]["Price"])
        self.assertEqual("An item that prevents weak wild Pokémon from appearing for 200 steps after its use.",
                         ini_data["SUPERREPEL"]["Description"])
        self.assertEqual("Repel,Fling_30", ini_data["MAXREPEL"]["Flags"])

    def test_iterate_items(self):
        ini_data = IniData(os.path.join(ROOT_DIR, "resources/items.txt"))
        names = []
        for item in ini_data:
            names.append(item[0])

        self.assertGreater(len(names), 0)

    def test_parse_items(self):
        item_data = ItemData(os.path.join(ROOT_DIR, "resources/items.txt"), None, None, None)
        print(item_data.to_json())
        self.assertNotEqual('', item_data.to_json())

    def test_parse_pokemon_data(self):
        stats = {
            "HP": Stat(Name("HP"), Text("HP"), Text("HP"), PokemonStatType.MAIN, 0),
            "ATTACK": Stat(Name("ATTACK"), Text("Attack"), Text("Atk"), PokemonStatType.MAIN_BATTLE, 1),
            "DEFENSE": Stat(Name("DEFENSE"), Text("Defense"), Text("Def"), PokemonStatType.MAIN_BATTLE, 2),
            "SPECIAL_ATTACK": Stat(Name("SPECIAL_ATTACK"), Text("Special Attack"), Text("SpAtk"),
                                   PokemonStatType.MAIN_BATTLE, 4),
            "SPECIAL_DEFENSE": Stat(Name("SPECIAL_DEFENSE"), Text("Special Defense"), Text("SpDef"),
                                    PokemonStatType.MAIN_BATTLE, 5),
            "SPEED": Stat(Name("SPEED"), Text("Speed"), Text("Spd"), PokemonStatType.MAIN_BATTLE, 3)
        }

        pokemon_data = SpeciesData(os.path.join(ROOT_DIR, "resources/pokemon.txt"), None, None, None,
                                   stats, None, None, None, None,
                                   None, None, None, None)
        self.assertNotEqual('', pokemon_data.to_json())


if __name__ == '__main__':
    result = run_test_with_coverage(TestIniData, __file__)
