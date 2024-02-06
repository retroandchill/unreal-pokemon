from typing import Optional

from pokemon.data_loader.pbs_ini_data import PbsIniData, DataContainer


class MoveData(PbsIniData):
    def __init__(self, config_path: str, damage_category_enum: DataContainer, target_ids: DataContainer):
        self.__schema = {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "Type": ("Type", "e", None),
            "Category": ("Category", "e", damage_category_enum),
            "Power": ("Power", "u", None),
            "Accuracy": ("Accuracy", "u", None),
            "TotalPP": ("TotalPP", "u", None),
            "Target": ("SellPrice", "e", target_ids),
            "Priority": ("Priority", "i", None),
            "FunctionCode": ("FunctionCode", "s", None),
            "Flags": ("Tags", "*s", None),
            "EffectChance": ("Consumable", "u", None),
            "Description": ("Description", "q", None)
        }
        super().__init__(config_path)

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        if "Target" in data and data["Target"].lower() == "none":
            data["Target"] = "NoTarget"

    def get_schema(self) -> dict[str, tuple[str, str, Optional[set[str]]]]:
        return self.__schema

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[DataContainer]]]):
        item["RealName"] = item.get("RealName", "Unnamed")
        item["Type"] = item.get("Type", "")
        item["Category"] = item.get("Category", "Status")
        item["Power"] = item.get("Power", 0)
        item["Accuracy"] = item.get("Accuracy", 100)
        item["TotalPP"] = item.get("TotalPP", 5)
        item["Target"] = item.get("Target", "NoTarget")
        item["Priority"] = item.get("Priority", 0)
        item["FunctionCode"] = item.get("FunctionCode", "")
        item["Tags"] = item.get("Tags", [])
        item["EffectChance"] = item.get("Consumable", 0)
        item["Description"] = item.get("Description", "???")