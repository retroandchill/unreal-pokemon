# ====================================================================================================================
# ** Unreal Pokémon created by Retro & Chill
# --------------------------------------------------------------------------------------------------------------------
# This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
# from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
# property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
# --------------------------------------------------------------------------------------------------------------------
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# ====================================================================================================================
from typing import Optional

from unreal import Stat

from .pbs_ini_data import DataContainer, PbsIniData
from ..ini_data import IniData
from ..unreal_data_loader import UnrealDataLoader

SpeciesArgs = tuple[
    DataContainer, DataContainer, DataContainer, dict[
        str, Stat], DataContainer, DataContainer, DataContainer, DataContainer, DataContainer, DataContainer, DataContainer]


class SpeciesData(PbsIniData[SpeciesArgs]):
    """Represents the translated species data imported from pokemon.txt"""

    def __init__(self, config_path: str, type_ids: DataContainer, gender_ratio_ids: DataContainer,
                 growth_rate_ids: DataContainer, stat_ids: dict[str, Stat], ability_ids: DataContainer,
                 move_ids: DataContainer, egg_group_ids: DataContainer, item_ids: DataContainer,
                 body_color_ids: DataContainer, body_shape_ids: DataContainer, habitat_ids: DataContainer,
                 evolution_ids: DataContainer):
        self.__stat_ids: dict[str, Stat] = stat_ids
        super().__init__(config_path, (type_ids, gender_ratio_ids, growth_rate_ids, stat_ids, ability_ids,
                                       move_ids, egg_group_ids, item_ids, body_color_ids, body_shape_ids,
                                       habitat_ids, evolution_ids))

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        # No additional processing needed on items
        pass

    def get_schema(self, ini_data: IniData, args: SpeciesArgs) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        key_loader = UnrealDataLoader(lambda: ini_data.get_keys())
        type_ids = args[0]
        gender_ratio_ids = args[1]
        growth_rate_ids = args[2]
        ability_ids = args[4]
        move_ids = args[5]
        egg_group_ids = args[6]
        item_ids = args[7]
        body_color_ids = args[8]
        body_shape_ids = args[9]
        habitat_ids = args[10]
        evolution_ids = args[11]

        return {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "FormName": ("FormName", "q", None),
            "Types": ("Types", "*e", type_ids),
            "BaseStats": ("BaseStats", "vvvvvv", None),
            "GenderRatio": ("GenderRatio", "e", gender_ratio_ids),
            "GrowthRate": ("GrowthRate", "e", growth_rate_ids),
            "BaseExp": ("BaseExp", "v", None),
            "EVs": ("EVs", "*ev", self.__stat_ids),
            "CatchRate": ("CatchRate", "u", None),
            "Happiness": ("Happiness", "u", None),
            "Abilities": ("Abilities", "*e", ability_ids),
            "HiddenAbilities": ("HiddenAbilities", "*e", ability_ids),
            "Moves": ("Moves", "*ue", None, move_ids),
            "TutorMoves": ("TutorMoves", "*e", move_ids),
            "EggMoves": ("EggMoves", "*e", move_ids),
            "EggGroups": ("EggGroups", "*e", egg_group_ids),
            "HatchSteps": ("HatchSteps", "v", None),
            "Incense": ("Incense", "e", item_ids),
            "Offspring": ("Offspring", "*e", key_loader),
            "Height": ("Height", "f", None),
            "Weight": ("Weight", "f", None),
            "Color": ("Color", "e", body_color_ids),
            "Shape": ("Shape", "e", body_shape_ids),
            "Habitat": ("Habitat", "e", habitat_ids),
            "Category": ("Category", "s", None),
            "Pokedex": ("PokedexEntry", "q", None),
            "Generation": ("Generation", "i", None),
            "Flags": ("Tags", "*s", None),
            "WildItemCommon": ("WildItemCommon", "*e", item_ids),
            "WildItemUncommon": ("WildItemUncommon", "*e", item_ids),
            "WildItemRare": ("WildItemRare", "*e", item_ids),
            "Evolutions": ("Evolutions", "^seS", key_loader, evolution_ids)
        }

    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[DataContainer]]]):
        item["RealName"] = item.get("RealName", "Unnamed")
        item["FormName"] = item.get("FormName", "")
        item["Category"] = item.get("Category", "???")
        item["PokedexEntry"] = item.get("PokedexEntry", "???")
        item["Types"] = item.get("Types", ["NORMAL"])
        item["BaseStats"] = item.get("BaseStats", {})
        item["EVs"] = item.get("EVs", {})

        stats_in_order = list(self.__stat_ids.values())
        stats_in_order.sort(key=lambda stat: stat.pbs_order, reverse=True)

        if isinstance(item["BaseStats"], list):
            formated_base_stats = {}
            for i in range(0, len(item["BaseStats"])):
                formated_base_stats[stats_in_order[i].id] = item["BaseStats"][i]

            item["BaseStats"] = formated_base_stats


        for s in self.__stat_ids:
            if s not in item["BaseStats"] or item["BaseStats"][s] <= 0:
                item["BaseStats"][s] = 1

            if s not in item["EVs"] or item["EVs"][s] < 0:
                item["EVs"][s] = 0

        item["BaseExp"] = item.get("BaseExp", 100)
        item["GrowthRate"] = item.get("GrowthRate", "Medium")
        item["GenderRatio"] = item.get("GenderRatio", "Female50Percent")
        item["CatchRate"] = item.get("CatchRate", 255)
        item["Happiness"] = item.get("Happiness", 70)
        item["Moves"] = item.get("Moves", [])
        item["TutorMoves"] = item.get("TutorMoves", [])
        item["EggMoves"] = item.get("EggMoves", [])
        item["Abilities"] = item.get("Abilities", [])
        item["HiddenAbilities"] = item.get("HiddenAbilities", [])
        item["WildItemCommon"] = item.get("WildItemCommon", [])
        item["WildItemUncommon"] = item.get("WildItemUncommon", [])
        item["WildItemRare"] = item.get("WildItemRare", [])
        item["EggGroups"] = item.get("EggGroups", ["Undiscovered"])
        item["HatchSteps"] = item.get("HatchSteps", 1)
        item["Incense"] = item.get("Incense", "")
        item["Offspring"] = item.get("Offspring", [])
        item["Evolution"] = item.get("Evolution", [])
        item["Height"] = item.get("Height", 1.0)
        item["Weight"] = item.get("Weight", 1.0)
        item["Color"] = item.get("Color", "Red")
        item["Shape"] = item.get("Shape", "Head")
        item["Habitat"] = item.get("Habitat", "NoHabitat")
        item["Generation"] = item.get("Generation", 0)
        item["Tags"] = item.get("Tags", [])
