import abc
import json
from typing import Optional

from pokemon.data_loader.ini_data import IniData
from pokemon.data_loader.schema_parser import convert_data_to_json
from pokemon.data_loader.unreal_data_loader import DataContainer


class PbsIniData(abc.ABC):
    def __init__(self, config_path: str):
        ini_data = IniData(config_path)
        self.__data = []
        schema = self.get_schema()
        for section_name, data in ini_data:
            self._preprocess_data(section_name, data)
            item = convert_data_to_json(section_name, data, schema)
            self._fix_data(item, schema)
            self.__data.append(item)

    @abc.abstractmethod
    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        pass

    @abc.abstractmethod
    def get_schema(self) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        pass

    @abc.abstractmethod
    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]) -> dict[str, any]:
        pass

    @staticmethod
    def _get_key(item: dict[str, any]) -> str:
        return item["Name"]

    def get_keys(self) -> set[str]:
        return set(map(self._get_key, self.__data))

    def to_json(self) -> str:
        return json.dumps(self.__data)
