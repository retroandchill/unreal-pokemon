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

from .pbs_ini_data import PbsIniData, DataContainer

MoveArgs = tuple[DataContainer, DataContainer, DataContainer]


class MoveData(PbsIniData[MoveArgs]):
    """Represents the translated move data imported from moves.txt"""
    def __init__(self, config_path: str, type_ids: DataContainer, damage_category_enum: DataContainer,
                 target_ids: DataContainer):
        super().__init__(config_path, (type_ids, damage_category_enum, target_ids))

    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        if "Target" in data and data["Target"].lower() == "none":
            data["Target"] = "NoTarget"

    def get_schema(self, ini_data, args: MoveArgs) -> dict[str, tuple[str, str, Optional[set[str]]]]:
        type_ids = args[0]
        damage_category_enum = args[1]
        target_ids = args[2]
        return {
            "SectionName": ("ID", "m", None),
            "Name": ("RealName", "s", None),
            "Type": ("Type", "e", type_ids),
            "Category": ("Category", "e", damage_category_enum),
            "Power": ("Power", "u", None),
            "Accuracy": ("Accuracy", "u", None),
            "TotalPP": ("TotalPP", "u", None),
            "Target": ("Target", "e", target_ids),
            "Priority": ("Priority", "i", None),
            "FunctionCode": ("FunctionCode", "s", None),
            "Flags": ("Tags", "*s", None),
            "EffectChance": ("EffectChance", "u", None),
            "Description": ("Description", "q", None)
        }

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
