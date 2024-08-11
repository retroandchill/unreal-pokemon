# "Unreal Pokémon" created by Retro & Chill.
import abc
import json
from typing import Optional, Generic, TypeVar

from pokemon.data_loader.ini_data import IniData
from pokemon.data_loader.schema_parser import convert_data_to_json
from pokemon.data_loader.unreal_data_loader import DataContainer

T = TypeVar('T')


class PbsIniData(Generic[T], abc.ABC):
    """Abstract class used for parsing PBS files that resemble a .ini file"""

    def __init__(self, config_path: str, args: T):
        ini_data = IniData(config_path)
        self.__data = []
        schema = self.get_schema(ini_data, args)
        for section_name, data in ini_data:
            self._preprocess_data(section_name, data)
            item = convert_data_to_json(section_name, data, schema)
            self._fix_data(item, schema)
            self.__data.append(item)

    @abc.abstractmethod
    def _preprocess_data(self, section_name: str, data: dict[str, str]) -> None:
        """Preprocesses the data that is received from the raw PBS file
        :param section_name: The name of the section parsed from the file
        :param data: The dictionary represeting the data, as unformatted strings
        """
        pass

    @abc.abstractmethod
    def get_schema(self, ini_data: IniData, args: T) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        """Get the schema used to parse the individual ini entries
        :param ini_data: The raw ini data that was passed
        :param args: The additional arguments that were passed to the constructor
        :return: The schema to parse the individual ini entries
        """
        pass

    @abc.abstractmethod
    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]) -> None:
        """
        Fix any issues with the data after it has been parsed
        :param item: The item that is to be fixed if needed
        :param schema: The original schema used to parse the item
        """
        pass

    @staticmethod
    def _get_key(item: dict[str, any]) -> str:
        """Get the key of the given item
        :param item: The item to extract the key from
        :return: The key of the given item
        """
        return item["Name"]

    def get_keys(self) -> set[str]:
        """
        Get the keys of each of the data entries
        :return: An unordered set of keys
        """
        return set(map(self._get_key, self.__data))

    def to_json(self) -> str:
        """
        Convert the list of data to a JSON string
        :return: A JSON representation of the data
        """
        return json.dumps(self.__data, indent=4)

    def __iter__(self):
        return self.__data.__iter__()
