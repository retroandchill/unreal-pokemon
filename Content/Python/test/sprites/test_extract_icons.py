import unittest

from coverage_helpers import run_test_with_coverage
from unreal import Texture2D, EditorAssetLibrary

import pokemon_icon_creator


class TestExtractIcons(unittest.TestCase):
    def test_extract_icons(self):
        texture1 = EditorAssetLibrary.load_asset('/Game/Python/test/resources/Icons/_Source/BULBASAUR.BULBASAUR')
        self.assertIsInstance(texture1, Texture2D)
        texture2 = EditorAssetLibrary.load_asset('/Game/Python/test/resources/Icons/_Source/CHARMANDER.CHARMANDER')
        self.assertIsInstance(texture2, Texture2D)
        texture3 = EditorAssetLibrary.load_asset('/Game/Python/test/resources/Icons/_Source/SQUIRTLE.SQUIRTLE')
        self.assertIsInstance(texture3, Texture2D)

        pokemon_icon_creator.execute([texture1, texture2, texture3], 2, 12)
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Icons/BULBASAUR.BULBASAUR'))
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Icons/CHARMANDER.CHARMANDER'))
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Icons/SQUIRTLE.SQUIRTLE'))

if __name__ == '__main__':
    result = run_test_with_coverage(TestExtractIcons, __file__)
