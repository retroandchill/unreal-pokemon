from typing import Optional

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
        item["RealName"] = item.get("RealName", "Unnamed")
        item["RealNamePlural"] = item.get("RealNamePlural", "Unnamed")
        item["RealPortionName"] = item.get("RealPortionName", "")
        item["RealPortionNamePlural"] = item.get("RealPortionNamePlural", "")
        item["Pocket"] = item.get("Pocket", 1)
        item["Price"] = item.get("Price", 0)
        item["SellPrice"] = item.get("SellPrice", int(item["Price"] / 2))
        item["BPPrice"] = item.get("BPPrice", 1)
        item["FieldUse"] = item.get("FieldUse", "NoFieldUse")
        item["BattleUse"] = item.get("BattleUse", "NoBattleUse")
        item["Tags"] = item.get("Tags", [])
        item["Consumable"] = item.get("Consumable", not ItemData.__is_important(item))
        item["ShowQuantity"] = item.get("ShowQuantity", True)
        item["Move"] = item.get("Move", "")
        item["Description"] = item.get("Description", "???")

    @staticmethod
    def __is_important(item: dict[str, any]) -> bool:
        return ItemData.__is_key_item(item) or ItemData.__is_tm(item) or ItemData.__is_hm(item)

    @staticmethod
    def __is_key_item(item: dict[str, any]) -> bool:
        return "KeyItem" in item["Tags"]

    @staticmethod
    def __is_tm(item: dict[str, any]) -> bool:
        return item["FieldUse"] == "TM"

    @staticmethod
    def __is_hm(item: dict[str, any]) -> bool:
        return item["FieldUse"] == "HM"

