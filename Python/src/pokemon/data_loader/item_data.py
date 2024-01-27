import json
from typing import TypedDict, Optional
from enum import Enum

from pokemon.data_loader import IniData, convert_data_to_json
from pokemon.data_loader.pbs_ini_data import PbsIniData

FieldUse = {'NoFieldUse', 'OnPokemon', 'Direct', 'TM', 'HM', 'TR'}

BattleUse = {'NoBattleUse', 'OnPokemon', 'OnMove', 'OnBattler', 'OnFoe', 'Direct'}


class ItemData(PbsIniData):

    SCHEMA = {
        "SectionName": ("ID", "m", None),
        "Name": ("RealName", "s", None),
        "NamePlural": ("RealNamePlural", "s", None),
        "PortionName": ("RealPortionName", "s", None),
        "PortionNamePlural": ("RealPortionNamePlural", "s", None),
        "Pocket": ("Pocket", "v", None),
        "Price": ("Price", "u", None),
        "SellPrice": ("SellPrice", "u", None),
        "BPPrice": ("BPPrice", "u", None),
        "FieldUse": ("FieldUse", "e", FieldUse),
        "BattleUse": ("BattleUse", "e", BattleUse),
        "Flags": ("Tags", "*s", None),
        "Consumable": ("Consumable", "b", None),
        "ShowQuantity": ("ShowQuantity", "b", None),
        "Move": ("Move", "e", None),
        "Description": ("Description", "q", None)
    }

    def __init__(self, config_path: str):
        super().__init__(config_path)

    def get_schema(self) -> dict[str, tuple[str, str, Optional[set[str]]]]:
        return self.SCHEMA

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]):
        if "SellPrice" not in item and "Price" in item:
            item["SellPrice"] = item["Price"] / 2
