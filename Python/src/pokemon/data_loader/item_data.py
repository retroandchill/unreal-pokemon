import json
from typing import TypedDict

from pokemon.data_loader import IniData

Item = TypedDict("Item", {
    "ID": str,
    "RealName": str,
    "RealNamePlural": str,
    "RealPortionName": str,
    "RealPortionNamePlural": str,
    "Pocket": int,
    "Price": int,
    "SellPrice": int,
    "BPPrice": int,
    "FieldUse": str,
    "BattleUse": str,
    "Tags": list[str],
    "Consumable": bool,
    "ShowQuantity": bool,
    "Move": str,
    "Description": str
})

class ItemData:
    def __init__(self, config_path: str):
        ini_data = IniData(config_path)
        self.__data = []
        for id, data in ini_data:
            result: Item = {
                "ID": id,
                "RealName": data["Name"],
                "RealNamePlural": data["NamePlural"],
                "RealPortionName": data["PortionName"] if data["PortionName"] else "",
                "RealPortionNamePlural": data["PortionNamePlural"] if data["PortionNamePlural"] else "",
                "Pocket": int(data["Pocket"]),
                "Price": int(data["Price"]),
                "SellPrice": int(data["SellPrice"]) if data["SellPrice"] else int(data["Price"]) / 2,
                "BPPrice": int(data["BPPrice"]) if data["BPPrice"] else 0,
                "FieldUse": data["FieldUse"] if data["FieldUse"] else "NoFieldUse",
                "BattleUse": data["BattleUse"] if data["BattleUse"] else "NoBattleUse",
                "Tags": data["Tags"].split(',') if data["Tags"] else [],
                "Consumable": bool(data["Consumable"]) if data["Consumable"] else True,
                "ShowQuantity": bool(data["ShowQuantity"]) if data["ShowQuantity"] else True,
                "Move": bool(data["Move"]) if data["Move"] else "",
                "Description": data["Description"] if data["Description"] else ""
            }
            self.__data.append(result)

    def to_json(self) -> str:
        return json.dumps(self.__data)
