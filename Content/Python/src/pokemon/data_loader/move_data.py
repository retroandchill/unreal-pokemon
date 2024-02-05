from typing import Optional

from pokemon.data_loader import PbsIniData, DataContainer

DamageCategory = {"Physical", "Special", "Status"}


class MoveData(PbsIniData):
    SCHEMA = {
        "SectionName": ("ID", "m", None),
        "Name": ("RealName", "s", None),
        "Type": ("Type", "e", None),
        "Category": ("Category", "e", DamageCategory),
        "Power": ("Power", "u", None),
        "Accuracy": ("Accuracy", "u", None),
        "TotalPP": ("TotalPP", "u", None),
        "Target": ("SellPrice", "e", None),
        "Priority": ("Priority", "i", None),
        "FunctionCode": ("FunctionCode", "s", None),
        "Flags": ("Tags", "*s", None),
        "EffectChance": ("Consumable", "u", None),
        "Description": ("Description", "q", None)
    }

    def __init__(self, config_path: str):
        super().__init__(config_path)

    def get_schema(self) -> dict[str, tuple[str, str, Optional[set[str]]]]:
        return self.SCHEMA

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[DataContainer]]]) -> dict[
        str, any]:
        item["RealName"] = item.get("RealName", "Unnamed")
        item["Type"] = item.get("Type", "")
        item["Category"] = item.get("Category", "Status")
        item["Power"] = item.get("Power", 0)
        item["Accuracy"] = item.get("Accuracy", 100)
        item["TotalPP"] = item.get("TotalPP", 5)
        item["Target"] = item.get("Target", "")
        item["Priority"] = item.get("Priority", 0)
        item["FunctionCode"] = item.get("FunctionCode", "")
        item["Tags"] = item.get("Tags", [])
        item["EffectChance"] = item.get("Consumable", 0)
        item["Description"] = item.get("Description", "???")
