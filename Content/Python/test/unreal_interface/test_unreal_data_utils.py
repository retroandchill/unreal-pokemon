
import unittest

from unreal import PokemonStatType, Stat, Name, Text

from coverage_helpers import run_test_with_coverage
from pokemon.unreal_interface.unreal_data_utils import enum_values, stat_entries

MAIN_BATTLE = 'MainBattle'


class TestUnrealDataUtils(unittest.TestCase):
    def test_get_enum_values(self):
        values = enum_values(PokemonStatType)
        self.assertEqual(values, {'Main', 'MainBattle', 'Battle'})

    def test_stat_entries(self):
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

        self.assertEqual(stat_entries('/Data/HardCoded', 'Stats'), stats)


if __name__ == '__main__':
    result = run_test_with_coverage(TestUnrealDataUtils, __file__)
