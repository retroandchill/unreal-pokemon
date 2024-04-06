import unittest

from coverage_helpers import run_test_with_coverage
from pokemon.data_loader.schema_parser import string_to_json_value


class TestSchemaParser(unittest.TestCase):
    def test_string_to_json_value(self):
        self.assertEqual(string_to_json_value('2', 'i'), 2)

        self.assertEqual(string_to_json_value('4', 'u'), 4)
        with self.assertRaises(ValueError):
            string_to_json_value('-3', 'u')

        self.assertEqual(string_to_json_value('8', 'v'), 8)
        with self.assertRaises(ValueError):
            string_to_json_value('-3', 'v')
        with self.assertRaises(ValueError):
            string_to_json_value('0', 'v')

        self.assertEqual(string_to_json_value('2a', 'x'), 0x2a)
        self.assertEqual(string_to_json_value('4.3', 'f'), 4.3)
        self.assertTrue(string_to_json_value('True', 'b'))

        self.assertEqual(string_to_json_value('field_name3', 'n'), 'field_name3')
        with self.assertRaises(ValueError):
            string_to_json_value('$invalid_name', 'n')
        self.assertEqual(string_to_json_value('field_name3', 's'), 'field_name3')
        self.assertEqual(string_to_json_value('field_name3', 'q'), 'field_name3')
        self.assertEqual(string_to_json_value('field_name3', 'm'), 'field_name3')
        self.assertEqual(string_to_json_value('EnumValue', 'e'), 'EnumValue')
        self.assertEqual(string_to_json_value('EnumValue', 'e', ['EnumValue', 'Test']), 'EnumValue')
        with self.assertRaises(ValueError):
            string_to_json_value('EnumValue', 'e', ['Test'])

        self.assertEqual(string_to_json_value('-3', 'y'), -3)
        self.assertEqual(string_to_json_value('EnumValue', 'y', ['EnumValue', 'Test']), 'EnumValue')


if __name__ == '__main__':
    result = run_test_with_coverage(TestSchemaParser, __file__)
