import unittest
from pokemon.data_loader import IniData


class TestIniData(unittest.TestCase):
    def test_get_item(self):
        ini_data = IniData("resources/items.txt")
        self.assertEqual("Repel", ini_data["REPEL"]["Name"])
        self.assertEqual("Repels", ini_data["REPEL"]["NamePlural"])
        self.assertEqual("700", ini_data["SUPERREPEL"]["Price"])
        self.assertEqual("An item that prevents weak wild Pokémon from appearing for 200 steps after its use.",
                         ini_data["SUPERREPEL"]["Description"])
        self.assertEqual("Repel,Fling_30", ini_data["MAXREPEL"]["Flags"])

    def test_iterate_items(self):
        ini_data = IniData("resources/items.txt")
        names = []
        for item in ini_data:
            names.append(item[0])

        self.assertEqual(["REPEL", "SUPERREPEL", "MAXREPEL"], names)


if __name__ == '__main__':
    unittest.main()
