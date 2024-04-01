# "Unreal Pokémon" created by Retro & Chill.
import unittest
from unittest.mock import MagicMock

from pokemon.data_loader import UnrealDataLoader


class MyTestCase(unittest.TestCase):
    def test_load_data(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader(create_data)
        for val in data:
            self.assertIn(val, data_loader)

        create_data.assert_called_once()

    def test_set_getter_later(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader()
        data_loader.set_getter(create_data)
        for val in data:
            self.assertIn(val, data_loader)

        create_data.assert_called_once()

    def test_invalid_lazy_loader(self):
        data_loader = UnrealDataLoader()
        with self.assertRaises(AttributeError):
            if 'item1' in data_loader:
                pass

    def test_double_set_getter(self):
        data = {'item1', 'item2', 'item3'}
        create_data = MagicMock(return_value=data)
        data_loader = UnrealDataLoader()
        data_loader.set_getter(create_data)

        new_getter = MagicMock(return_value=set())
        with self.assertRaises(AttributeError):
            data_loader.set_getter(new_getter)


if __name__ == '__main__':
    unittest.main()
