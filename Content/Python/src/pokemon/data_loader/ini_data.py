# "Unreal Pokémon" created by Retro & Chill.
import re
from typing import Optional, Iterator, TextIO


class IniData:
    """Represents a raw parse of a PBS file that resembles the .ini format"""
    def __init__(self, config_path: str):
        with open(config_path, 'r', encoding='utf') as file:
            self.__parse_config(file)

    def __parse_config(self, file: TextIO) -> None:
        """
        Parses the .ini config file
        :param file: The text to parse the data from
        """
        self._items: dict[str, dict[str, str]] = {}
        item_id: Optional[str] = None
        for line in file:
            if line.startswith('#'):
                continue

            match = re.match(r'\[(\w+)]', line)
            if match is not None:
                item_id = match.group(1)
                self._items[item_id] = {}

            match = re.match(r'(\w+)\s*=\s*(.*)', line)
            if match is not None and item_id is not None:
                key = match.group(1)
                value = match.group(2).strip()
                self._items[item_id][key] = value

    def __iter__(self) -> Iterator[tuple[str, dict[str, str]]]:
        return iter(self._items.items())

    def __getitem__(self, item_id: str) -> dict[str, str]:
        return self._items[item_id]

    def get_keys(self) -> set[str]:
        """
        Returns a set of keys from the ini data
        :return: An unordered set of keys
        """
        return set(self._items.keys())
