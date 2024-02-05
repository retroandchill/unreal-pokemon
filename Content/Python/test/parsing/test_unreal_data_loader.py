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


if __name__ == '__main__':
    unittest.main()
