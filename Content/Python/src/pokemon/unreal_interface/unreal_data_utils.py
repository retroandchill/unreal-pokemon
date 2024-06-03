# "Unreal Pokémon" created by Retro & Chill.
import re
from typing import Type

from unreal import EnumBase, Name, EditorAssetLibrary, DataTable, DataTableFunctionLibrary, Stat, ImportUtils


def enum_values(enum: Type[EnumBase]) -> set[str]:
    """
    Parse an Unreal enumeration into a set of string keys
    :param enum: The enum to parse the keys from
    :return: The unordered set of keys
    """
    ret = set()
    pattern = re.compile(r'\s+')
    for attr in vars(enum):
        value = getattr(enum, attr)
        if isinstance(value, enum):
            display_name = str(value.get_display_name())
            ret.add(re.sub(pattern, '', display_name))

    return ret


def convert_name(name: Name) -> str:
    """
    Converts an Unreal Name object into a Python string
    :param name: The name object to convert
    :return: The Python string version
    """
    return str(name)


def data_table_values(directory_name: str, table_name: str) -> set[str]:
    """
    Gets the keys from an Unreal DataTable asset
    :param directory_name: The directory in the Content Browser to look in
    :param table_name: The name of the table as seen in the editor
    :return: The unordered set of keys from the table
    """
    data_table = EditorAssetLibrary.load_asset('/{0}/{1}.{1}'.format(directory_name, table_name))
    if isinstance(data_table, DataTable):
        return set(map(convert_name, DataTableFunctionLibrary.get_data_table_row_names(data_table)))
    else:
        raise ValueError("Unexpected data table name")


def stat_entries(directory_name: str, table_name: str) -> dict[str, Stat]:
    """
    Gets a map of data table row names to their entries
    :param directory_name: The directory in the Content Browser to look in
    :param table_name: The name of the table as seen in the editor
    :return: The map of data table row names to their entries
    """

    data_table = EditorAssetLibrary.load_asset('/{0}/{1}.{1}'.format(directory_name, table_name))
    if isinstance(data_table, DataTable):
        rows = ImportUtils.get_main_stats_from_table(data_table)
        ret = {}
        for row in rows:
            ret[str(row.id)] = row

        return ret
    else:
        raise ValueError("Unexpected data table name")


def pocket_names() -> dict[int, str]:
    """
    Get out the map of pocket names from Unreal and convert them to a dictionary
    :return: The map of pocket IDs to pocket names
    """
    ret = {}
    for key, value in ImportUtils.get_pocket_names().items():
        ret[key] = str(value)
    return ret
