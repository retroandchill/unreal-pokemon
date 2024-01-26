import json
from typing import TypedDict
from enum import Enum

from pokemon.data_loader import IniData, convert_data_to_json

FieldUse = {'NoFieldUse', 'OnPokemon', 'Direct', 'TM', 'HM', 'TR'}

BattleUse = {'NoBattleUse', 'OnPokemon', 'OnMove', 'OnBattler', 'OnFoe', 'Direct'}


class ItemData:
    SCHEMA = {
        "SectionName": ("m", None),
        "Name": ("s", None),
        "NamePlural": ("s", None),
        "PortionName": ("s", None),
        "PortionNamePlural": ("s", None),
        "Pocket": ("v", None),
        "Price": ("u", None),
        "SellPrice": ("u", None),
        "BPPrice": ("u", None),
        "FieldUse": ("e", FieldUse),
        "BattleUse": ("e", BattleUse),
        "Flags": ("*s", None),
        "Consumable": ("b", None),
        "ShowQuantity": ("b", None),
        "Move": ("e", None),
        "Description": ("q", None)
    }

    def __init__(self, config_path: str):
        ini_data = IniData(config_path)
        self.__data = []
        for section_name, data in ini_data:
            self.__data.append(convert_data_to_json(section_name, data, self.SCHEMA))

    def to_json(self) -> str:
        return json.dumps(self.__data)
