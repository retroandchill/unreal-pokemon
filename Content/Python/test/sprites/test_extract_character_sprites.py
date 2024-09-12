import unittest

from unreal import Texture2D, Array, EditorAssetLibrary, PaperZDAnimationSource

import character_sprite_creator
from coverage_helpers import run_test_with_coverage


class TestExtractSprites(unittest.TestCase):
    def test_extract_character_sprites(self):
        anim_source = EditorAssetLibrary.load_asset('/Game/Python/test/resources/ExpansionTest.ExpansionTest')
        self.assertIsInstance(anim_source, PaperZDAnimationSource)
        sprites: Array[Texture2D] = Array(Texture2D)

        sprite1 = EditorAssetLibrary.load_asset('/Game/Python/test/resources/NPC01.NPC01')
        self.assertIsInstance(sprite1, Texture2D)
        sprites.append(sprite1)

        sprite2 = EditorAssetLibrary.load_asset('/Game/Python/test/resources/NPC02.NPC02')
        self.assertIsInstance(sprite2, Texture2D)
        sprites.append(sprite2)

        character_sprite_creator.execute(sprites, 4, 16, 5.0, anim_source)
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/AnimSequences/NPC01.NPC01'))
        self.assertTrue(EditorAssetLibrary.does_asset_exist('/Game/Python/test/resources/AnimSequences/NPC02.NPC02'))



if __name__ == '__main__':
    result = run_test_with_coverage(TestExtractSprites, __file__)
