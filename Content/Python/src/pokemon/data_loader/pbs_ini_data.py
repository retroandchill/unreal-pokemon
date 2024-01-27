import abc
import json
from typing import Optional

from pokemon.data_loader import IniData, convert_data_to_json


class PbsIniData:
    def __init__(self, config_path: str):
        ini_data = IniData(config_path)
        self.__data = []
        schema = self.get_schema()
        for section_name, data in ini_data:
            item = convert_data_to_json(section_name, data, schema)
            self._fix_data(item, schema)
            self.__data.append(item)

    @abc.abstractmethod
    def get_schema(self) -> dict[str, tuple[str, str, Optional[set[str]]]]:
        pass

    @abc.abstractmethod
    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]) -> dict[str, any]:
        pass

    def to_json(self) -> str:
        return json.dumps(self.__data)
