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
