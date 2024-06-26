# "Unreal Pokémon" created by Retro & Chill.
import unreal
from unreal import DataTable, DataTableFunctionLibrary, EditorAssetLibrary, ScriptStruct, GameplayTagTableRow

from pokemon.data_loader.pbs_data import PbsIniData, ItemData, MoveData, TypeData, AbilityData, SpeciesData, \
    TrainerTypeData


def import_data(item_data: PbsIniData, table_name: str, struct_type: ScriptStruct) -> None:
    """
    Import data from the given PBS data and insert it into a DataTable asset
    :param item_data: The PBS data to import
    :param table_name: The name of the data table as seen in the Data folder of the Content browser
    :param struct_type: The struct used for the row import
    """
    data_table = EditorAssetLibrary.load_asset('/Game/Data/{0}.{0}'.format(table_name))
    if isinstance(data_table, DataTable):
        DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json(), struct_type)


def import_types(type_data: TypeData) -> None:
    """
    Import type data into Unreal
    :param type_data: The list of types to import
    """
    print("Importing types...")
    import_data(type_data, "Types", unreal.Type.static_struct())


def import_moves(move_data: MoveData) -> None:
    """
    Import move data into Unreal
    :param move_data: The list of moves to import
    """
    print("Importing moves...")
    import_data(move_data, "Moves", unreal.MoveData.static_struct())


def import_items(item_data: ItemData) -> None:
    """
    Import item data into Unreal
    :param item_data: The list of items to import
    """
    print("Importing items...")
    import_data(item_data, "Items", unreal.Item.static_struct())


def import_abilities(ability_data: AbilityData) -> None:
    """
    Import ability data into Unreal
    :param ability_data: The list of abilities to import
    """
    print("Importing abilities...")
    import_data(ability_data, "Abilities", unreal.Ability.static_struct())


def import_species(species_data: SpeciesData) -> None:
    """
    Import species data into Unreal
    :param species_data: The list of species to import
    """
    print("Importing species...")
    import_data(species_data, "Pokemon", unreal.SpeciesData.static_struct())


def import_trainer_types(trainer_type_data: TrainerTypeData) -> None:
    """
    Import trainer type data into Unreal
    :param trainer_type_data: The list of trainer types to import
    """
    print("Importing trainer types...")
    import_data(trainer_type_data, "TrainerTypes", unreal.TrainerType.static_struct())
