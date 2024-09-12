import unittest

from coverage_helpers import run_test_with_coverage
from unreal import EditorAssetLibrary

import pokemon_battle_sprite_creator
import os.path


class TestExtractBattleSprites(unittest.TestCase):
    def text_extract_sprites(self):
        manifest_file_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'resources', 'sprite_data.csv'))
        pokemon_battle_sprite_creator.execute('/Game/Python/test/resources', manifest_file_path, 12)
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Front/ABOMASNOW.ABOMASNOW'))
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Front/ABRA.ABRA'))
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/Front/ABSOL.ABSOL'))



if __name__ == '__main__':
    result = run_test_with_coverage(TestExtractBattleSprites, __file__)
