import os

from unreal import Paths

from pokemon.data_loader import ItemData, MoveData
from pokemon.data_writer import import_items

if __name__ == "__main__":
    base_path = Paths.project_dir()
    data_dir = "/Game/Content/Data"
    pbs_dir = os.path.join(base_path, "PBS")

    moves = MoveData(os.path.join(pbs_dir, "moves.xt"))
    items = ItemData(os.path.join(pbs_dir, "items.txt"))
    import_items(items)
