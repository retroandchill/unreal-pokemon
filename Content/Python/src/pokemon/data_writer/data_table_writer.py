from unreal import DataTable, DataTableFunctionLibrary, EditorAssetLibrary

from pokemon.data_loader import PbsIniData, ItemData, MoveData, TypeData


def import_data(item_data: PbsIniData, table_name: str):
    eal = EditorAssetLibrary()
    data_table = eal.load_asset('/Game/Data/{0}.{0}'.format(table_name))
    if isinstance(data_table, DataTable):
        DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json())


def import_types(type_data: TypeData):
    print("Importing moves...")
    import_data(type_data, "Types")


def import_moves(move_data: MoveData):
    print("Importing moves...")
    import_data(move_data, "Moves")


def import_items(item_data: ItemData):
    print("Importing items...")
    import_data(item_data, "Items")
