import unittest

from coverage_helpers import run_test_with_coverage
from pokemon.unreal_interface.gameplay_tags import create_gameplay_tag_structure


class TestGameplayTags(unittest.TestCase):
    def test_create_gameplay_tags(self):
        result = create_gameplay_tag_structure('Battle.Items.BattleUse', ['HPPPRestore'])
        self.assertEqual(1, len(result['GameplayTags']))
        self.assertEqual('Battle.Items.BattleUse.HPPPRestore', result['GameplayTags'][0]['TagName'])
        self.assertEqual(3, len(result['ParentTags']))
        self.assertEqual('Battle.Items.BattleUse', result['ParentTags'][0]['TagName'])
        self.assertEqual('Battle.Items', result['ParentTags'][1]['TagName'])
        self.assertEqual('Battle', result['ParentTags'][2]['TagName'])

        result = create_gameplay_tag_structure('Battle.Items.BattleUse', [])
        self.assertEqual(0, len(result['GameplayTags']))
        self.assertEqual(0, len(result['ParentTags']))


if __name__ == '__main__':
    result = run_test_with_coverage(TestGameplayTags, __file__)
