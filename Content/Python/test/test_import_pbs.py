import os
import unittest

from unreal import Text, Paths

import import_pbs
from coverage_helpers import run_test_with_coverage


class TestImportPbs(unittest.TestCase):
    def test_import_pbs(self):
        pbs_dir = os.path.join(Paths.project_dir(), 'PBS')

        type_file = Text(os.path.join(pbs_dir, "types.txt"))
        move_file = Text(os.path.join(pbs_dir, "moves.txt"))
        item_file = Text(os.path.join(pbs_dir, "items.txt"))
        ability_file = Text(os.path.join(pbs_dir, "abilities.txt"))
        species_file = Text(os.path.join(pbs_dir, "pokemon.txt"))
        trainer_types_file = Text(os.path.join(pbs_dir, "trainer_types.txt"))
        import_pbs.execute(type_file, move_file, item_file, ability_file, species_file, trainer_types_file)


if __name__ == '__main__':
    result = run_test_with_coverage(TestImportPbs, __file__)
