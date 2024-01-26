import os
import sys
sys.path.append(os.path.dirname(os.path.dirname(__file__)))
print(sys.path)

from unreal import Paths
from pokemon.data_loader import ItemData
from pokemon.data_writer import import_items

if __name__ == "__main__":
    base_path = Paths.project_dir()
    data_dir = os.path.join(base_path, "Content", "Data")
    pbs_dir = os.path.join(base_path, "PBS")

    items = ItemData(os.path.join(pbs_dir, "items.txt"))
    import_items(items, data_dir)
