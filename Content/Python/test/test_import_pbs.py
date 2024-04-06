import os
import sys
import unittest
from typing import Type, AnyStr
from unittest import TestLoader, TextTestRunner, TestResult, TestCase
from unittest.mock import MagicMock

from coverage import Coverage
from unreal import Text, Stat, Name, PokemonStatType, Paths

import import_pbs
from coverage_helpers import run_test_with_coverage


class TestImportPbs(unittest.TestCase):
    def test_import_pbs(self):
        stats = {
            "HP": Stat(Name("HP"), Text("HP"), Text("HP"), PokemonStatType.MAIN, 0),
            "ATTACK": Stat(Name("ATTACK"), Text("Attack"), Text("Atk"), PokemonStatType.MAIN_BATTLE, 1),
            "DEFENSE": Stat(Name("DEFENSE"), Text("Defense"), Text("Def"), PokemonStatType.MAIN_BATTLE, 2),
            "SPECIAL_ATTACK": Stat(Name("SPECIAL_ATTACK"), Text("Special Attack"), Text("SpAtk"),
                                   PokemonStatType.MAIN_BATTLE, 4),
            "SPECIAL_DEFENSE": Stat(Name("SPECIAL_DEFENSE"), Text("Special Defense"), Text("SpDef"),
                                    PokemonStatType.MAIN_BATTLE, 5),
            "SPEED": Stat(Name("SPEED"), Text("Speed"), Text("Spd"), PokemonStatType.MAIN_BATTLE, 3)
        }

        import_pbs.data_table_values = MagicMock(return_value=None)
        import_pbs.import_types = MagicMock()
        import_pbs.import_moves = MagicMock()
        import_pbs.import_items = MagicMock()
        import_pbs.import_abilities = MagicMock()
        import_pbs.import_species = MagicMock()
        import_pbs.import_trainer_types = MagicMock()
        import_pbs.stat_entries = MagicMock(return_value=stats)

        pbs_dir = os.path.join(os.path.dirname(__file__), '..', '..', '..', 'PBS')

        type_file = Text(os.path.join(pbs_dir, "types.txt"))
        move_file = Text(os.path.join(pbs_dir, "moves.txt"))
        item_file = Text(os.path.join(pbs_dir, "items.txt"))
        ability_file = Text(os.path.join(pbs_dir, "abilities.txt"))
        species_file = Text(os.path.join(pbs_dir, "pokemon.txt"))
        trainer_types_file = Text(os.path.join(pbs_dir, "trainer_types.txt"))
        import_pbs.execute(type_file, move_file, item_file, ability_file, species_file, trainer_types_file)

        import_pbs.import_types.assert_called_once()
        import_pbs.import_moves.assert_called_once()
        import_pbs.import_items.assert_called_once()
        import_pbs.import_abilities.assert_called_once()
        import_pbs.import_species.assert_called_once()
        import_pbs.import_trainer_types.assert_called_once()


if __name__ == '__main__':
    result = run_test_with_coverage(TestImportPbs, __file__)
