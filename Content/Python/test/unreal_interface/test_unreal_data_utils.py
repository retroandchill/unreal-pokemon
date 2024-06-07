
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
        self.assertEqual(len(stat_entries('/PokemonStarterKit/Data', 'Stats')), 6)


if __name__ == '__main__':
    result = run_test_with_coverage(TestUnrealDataUtils, __file__)
