import os

from pokemon.data_loader import IniData, ItemData
from unreal import AssetImportTask, DataTable, DataTableFunctionLibrary, EditorAssetLibrary


def import_items(item_data: ItemData, target_dir: str):
    ue_asset_path = os.path.join(target_dir, "Items.uasset")
    data_table = DataTable(EditorAssetLibrary.load_asset(ue_asset_path))

    DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json())
