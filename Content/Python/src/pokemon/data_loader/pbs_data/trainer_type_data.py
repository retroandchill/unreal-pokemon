# "Unreal Pokémon" created by Retro & Chill.
from typing import Optional

from pokemon.data_loader import IniData
from pokemon.data_loader.pbs_data import PbsIniData
from pokemon.data_loader.unreal_data_loader import DataContainer

TrainerTypeArgs = tuple[DataContainer]


class TrainerTypeData(PbsIniData[TrainerTypeArgs]):
    """Represents the translated trainer type data imported from trainer_types.txt"""

    def __init__(self, config_path: str, trainer_genders: DataContainer):
        super().__init__(config_path, (trainer_genders,))

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        # No additional processing needed on abilities
        pass

    def get_schema(self, ini_data: IniData, args: TrainerTypeArgs) -> dict[
        str, tuple[str, str, Optional[DataContainer]]]:
        gender_data = args[0]
        return {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "Gender": ("Gender", "e", gender_data),
            "BaseMoney": ("BaseMoney", "u", None),
            "SkillLevel": ("SkillLevel", "u", None),
            "Flags": ("Tags", "*e", None),
            "IntroBGM": ("IntroMusic", "s", None),
            "BattleBGM": ("BattleMusic", "s", None),
            "VictoryBGM": ("VictoryMusic", "s", None)
        }

    def _fix_data(self, trainer_type: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]):
        trainer_type["RealName"] = trainer_type.get("RealName", "Unnamed")
        trainer_type["Gender"] = trainer_type.get("Gender", "Unknown")
        trainer_type["BaseMoney"] = trainer_type.get("BaseMoney", 30)
        trainer_type["SkillLevel"] = trainer_type.get("SkillLevel", trainer_type["BaseMoney"])
        trainer_type["Tags"] = trainer_type.get("Tags", [])
        trainer_type["IntroMusic"] = trainer_type.get("IntroMusic", None)
        trainer_type["BattleMusic"] = trainer_type.get("BattleMusic", None)
        trainer_type["VictoryMusic"] = trainer_type.get("VictoryMusic", None)
