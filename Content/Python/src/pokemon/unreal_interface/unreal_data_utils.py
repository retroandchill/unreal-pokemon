import re
from typing import Type

from unreal import EnumBase, Name, EditorAssetLibrary, DataTable, DataTableFunctionLibrary


def enum_values(enum: Type[EnumBase]) -> set[str]:
    ret = set()
    pattern = re.compile(r'\s+')
    for attr in vars(enum):
        value = getattr(enum, attr)
        if isinstance(value, enum):
            display_name = str(value.get_display_name())
            ret.add(re.sub(pattern, '', display_name))

    return ret


def convert_name(name: Name) -> str:
    return str(name)


def data_table_values(directory_name: str, table_name: str) -> set[str]:
    eal = EditorAssetLibrary()
    data_table = eal.load_asset('/Game/{0}/{1}.{1}'.format(directory_name, table_name))
    if isinstance(data_table, DataTable):
        return set(map(convert_name, DataTableFunctionLibrary.get_data_table_row_names(data_table)))
    else:
        raise ValueError("Unexpected data table name")
