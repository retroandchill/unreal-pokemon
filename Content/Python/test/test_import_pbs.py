import os
import unittest
from unittest.mock import MagicMock

from unreal import Text, Stat, Name, PokemonStatType

import import_pbs
import mocks

assert mocks, "Something is imported for its side effects."

MAIN_BATTLE = 'MainBattle'


class MyTestCase(unittest.TestCase):
    def test_import_pbs(self):
        stats = {
            "HP": Stat(Name("HP"), 0, Text("HP"), Text("HP"), PokemonStatType("Main")),
            "ATTACK": Stat(Name("ATTACK"), 1, Text("Attack"), Text("Atk"), PokemonStatType(MAIN_BATTLE)),
            "DEFENSE": Stat(Name("DEFENSE"), 2, Text("Defense"), Text("Def"), PokemonStatType(MAIN_BATTLE)),
            "SPECIAL_ATTACK": Stat(Name("SPECIAL_ATTACK"), 4, Text("Special Attack"), Text("SpAtk"),
                                   PokemonStatType(MAIN_BATTLE)),
            "SPECIAL_DEFENSE": Stat(Name("SPECIAL_DEFENSE"), 5, Text("Special Defense"), Text("SpDef"),
                                    PokemonStatType(MAIN_BATTLE)),
            "SPEED": Stat(Name("SPEED"), 3, Text("Speed"), Text("Spd"), PokemonStatType(MAIN_BATTLE))
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
    unittest.main()
