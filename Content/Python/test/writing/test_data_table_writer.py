import unittest
from unittest.mock import MagicMock

import mocks

from unreal import DataTable, EditorAssetLibrary, DataTableFunctionLibrary

from pokemon.data_writer import import_types, import_moves, import_items, import_abilities
from pokemon.data_writer.data_table_writer import import_species, import_trainer_types

assert mocks, "Something is imported for its side effects."

JSON_EXAMPLE = '''
{  
    "employee": {  
        "name":       "sonoo",   
        "salary":      56000,   
        "married":    true  
    }  
}  
'''


class MyTestCase(unittest.TestCase):
    def test_import_types(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        type_data = MagicMock()
        type_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_types(type_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/Types.Types')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)

    def test_import_moves(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        move_data = MagicMock()
        move_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_moves(move_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/Moves.Moves')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)

    def test_import_items(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        item_data = MagicMock()
        item_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_items(item_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/Items.Items')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)

    def test_import_abilities(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        ability_data = MagicMock()
        ability_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_abilities(ability_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/Abilities.Abilities')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)

    def test_import_species(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        species_data = MagicMock()
        species_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_species(species_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/Pokemon.Pokemon')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)

    def test_import_trainer_types(self):
        test_table = DataTable()
        EditorAssetLibrary.load_asset = MagicMock(return_value=test_table)
        DataTableFunctionLibrary.fill_data_table_from_json_string = MagicMock()

        species_data = MagicMock()
        species_data.to_json = MagicMock(return_value=JSON_EXAMPLE)
        import_trainer_types(species_data)
        EditorAssetLibrary.load_asset.assert_called_once_with('/Game/Data/TrainerTypes.TrainerTypes')
        DataTableFunctionLibrary.fill_data_table_from_json_string.assert_called_once_with(test_table, JSON_EXAMPLE)


if __name__ == '__main__':
    unittest.main()
