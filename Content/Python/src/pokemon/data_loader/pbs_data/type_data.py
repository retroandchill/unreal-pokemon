from typing import Optional

from .pbs_ini_data import DataContainer, PbsIniData
from ..ini_data import IniData
from ..unreal_data_loader import UnrealDataLoader

TypeArgs = tuple


class TypeData(PbsIniData[TypeArgs]):
    def __init__(self, config_path: str):
        super().__init__(config_path, ())

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        # No additional processing needed on items
        pass

    def get_schema(self, ini_data: IniData, args: TypeArgs) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        key_loader = UnrealDataLoader(lambda: ini_data.get_keys())
        return {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "IconPosition": ("IconPosition", "u", None),
            "IsSpecialType": ("IsSpecialType", "b", None),
            "IsPseudoType": ("IsPseudoType", "b", None),
            "Weaknesses": ("Weaknesses", "*e", key_loader),
            "Resistances": ("Resistances", "*e", key_loader),
            "Immunities": ("Immunities", "*e", key_loader),
            "Flags": ("Tags", "*e", None),
        }

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]):
        item["RealName"] = item.get("RealName", "Unnamed")
        item["IconPosition"] = item.get("IconPosition", 0)
        item["IsSpecialType"] = item.get("IsSpecialType", False)
        item["IsPseudoType"] = item.get("IsPseudoType", False)
        item["Weaknesses"] = item.get("Weaknesses", [])
        item["Resistances"] = item.get("Resistances", [])
        item["Immunities"] = item.get("Immunities", [])
        item["Tags"] = item.get("Tags", [])
