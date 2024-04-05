# "Unreal Pokémon" created by Retro & Chill.
from typing import Optional

from .pbs_ini_data import PbsIniData, DataContainer
from ..ini_data import IniData

AbilityArgs = tuple


class AbilityData(PbsIniData[AbilityArgs]):
    """Represents the translated ability data imported from abilities.txt"""

    def __init__(self, config_path: str):
        super().__init__(config_path, ())

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        # No additional processing needed on abilities
        pass

    def get_schema(self, ini_data: IniData, args: AbilityArgs) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        return {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "Description": ("Description", "q", None),
            "Flags": ("Tags", "*e", None),
        }

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]):
        item["RealName"] = item.get("RealName", "Unnamed")
        item["Description"] = item.get("Description", "")
        item["Tags"] = item.get("Tags", [])
