import os

from unreal import Paths, FieldUse, BattleUse, MoveDamageCategory

from pokemon.data_loader import ItemData, MoveData, UnrealDataLoader, TypeData
from pokemon.data_writer import import_items, import_moves, import_types
from pokemon.unreal_interface import enum_values, data_table_values

if __name__ == "__main__":
    base_path = Paths.project_dir()
    data_dir = "/Game/Content/Data"
    pbs_dir = os.path.join(base_path, "PBS")

    types = TypeData(os.path.join(pbs_dir, "types.txt"))
    import_types(types)

    type_ids = UnrealDataLoader(types.get_keys)
    damage_category_enum = enum_values(MoveDamageCategory)
    target_ids = data_table_values("Data/Hardcoded", "Targets")
    moves = MoveData(os.path.join(pbs_dir, "moves.txt"), type_ids, damage_category_enum, target_ids)
    import_moves(moves)

    field_use_enum = enum_values(FieldUse)
    battle_use_enum = enum_values(BattleUse)
    move_ids = UnrealDataLoader(moves.get_keys)
    items = ItemData(os.path.join(pbs_dir, "items.txt"), field_use_enum, battle_use_enum, move_ids)
    import_items(items)
