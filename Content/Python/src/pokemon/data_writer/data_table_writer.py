import os

from unreal import AssetRegistry, DataTable, DataTableFunctionLibrary, EditorAssetLibrary

from pokemon.data_loader import ItemData


def import_items(item_data: ItemData):
    print("Importing items...")
    eal = EditorAssetLibrary()
    data_table = DataTable(eal.load_asset('/Game/Data/Items.Items'))

    DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json())
