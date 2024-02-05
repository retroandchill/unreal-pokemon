import os
import re
from typing import Type

from unreal import Paths, FieldUse, BattleUse, EnumBase

from pokemon.data_loader import ItemData, MoveData, UnrealDataLoader
from pokemon.data_writer import import_items


def enum_values(enum: Type[EnumBase]) -> set[str]:
    ret = set()
    pattern = re.compile(r'\s+')
    for attr in vars(enum):
        value = getattr(enum, attr)
        if isinstance(value, enum):
            display_name = str(value.get_display_name())
            ret.add(re.sub(pattern, '', display_name))

    return ret


if __name__ == "__main__":
    base_path = Paths.project_dir()
    data_dir = "/Game/Content/Data"
    pbs_dir = os.path.join(base_path, "PBS")

    moves = MoveData(os.path.join(pbs_dir, "moves.txt"))

    field_use_enum = enum_values(FieldUse)
    battle_use_enum = enum_values(BattleUse)
    move_ids = UnrealDataLoader(moves.get_keys)
    items = ItemData(os.path.join(pbs_dir, "items.txt"), field_use_enum, battle_use_enum, move_ids)
    import_items(items)
