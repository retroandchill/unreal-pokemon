import os
import sys
import unittest
from unittest import TestLoader, TextTestRunner
from unittest.mock import MagicMock

from coverage import Coverage
from unreal import PokemonStatType, DataTable, EditorAssetLibrary, DataTableFunctionLibrary, ImportUtils, Stat, Name, \
    Text, Paths

from coverage_helpers import run_test_with_coverage
from pokemon.unreal_interface.unreal_data_utils import enum_values, data_table_values, stat_entries

MAIN_BATTLE = 'MainBattle'


class TestUnrealDataUtils(unittest.TestCase):
    def test_get_enum_values(self):
        values = enum_values(PokemonStatType)
        self.assertEqual(values, {'Main', 'MainBattle', 'Battle'})

    def test_data_table_values(self):
        with self.assertRaises(ValueError):
            EditorAssetLibrary.load_asset = MagicMock(return_value=3)
            data_table_values('TestDir', 'TestTable')
            EditorAssetLibrary.load_asset.assert_called_once_with('/Game/TestDir/TestTable.TestTable')

        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.get_data_table_row_names = MagicMock(return_value=['Row1', "Row2"])
        self.assertEqual(data_table_values('TestDir', 'TestTable'), {'Row1', "Row2"})
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/TestDir/TestTable.TestTable')
        DataTableFunctionLibrary.get_data_table_row_names.assert_called_once_with(test_table)

    def test_stat_entries(self):
        stats = {
            "HP": Stat(Name("HP"), 0, Text("HP"), Text("HP"), PokemonStatType("Main")),
            "ATTACK": Stat(Name("ATTACK"), 1, Text("Attack"), Text("Atk"), PokemonStatType(MAIN_BATTLE)),
            "DEFENSE": Stat(Name("DEFENSE"), 2, Text("Defense"), Text("Def"), PokemonStatType(MAIN_BATTLE)),
            "SPECIAL_ATTACK": Stat(Name("SPECIAL_ATTACK"), 4, Text("Special Attack"), Text("SpAtk"),
                                   PokemonStatType(MAIN_BATTLE)),
            "SPECIAL_DEFENSE": Stat(Name("SPECIAL_DEFENSE"), 5, Text("Special Defense"), Text("SpDef"),
                                    PokemonStatType(MAIN_BATTLE)),
            "SPEED": Stat(Name("SPEED"), 3, Text("Speed"), Text("Spd"), PokemonStatType(MAIN_BATTLE))
        }

        with self.assertRaises(ValueError):
            EditorAssetLibrary.load_asset = MagicMock(return_value=3)
            stat_entries('TestDir', 'TestTable')
            EditorAssetLibrary.load_asset.assert_called_once_with('/Game/TestDir/TestTable.TestTable')

        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        ImportUtils.get_main_stats_from_table = MagicMock(return_value=stats.values())
        self.assertEqual(stat_entries('TestDir', 'TestTable'), stats)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/TestDir/TestTable.TestTable')
        ImportUtils.get_main_stats_from_table.assert_called_once_with(test_table)


if __name__ == '__main__':
    result = run_test_with_coverage(TestUnrealDataUtils, __file__)
