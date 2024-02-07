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
import abc
import json
from typing import Optional, Generic, TypeVar

from pokemon.data_loader.ini_data import IniData
from pokemon.data_loader.schema_parser import convert_data_to_json
from pokemon.data_loader.unreal_data_loader import DataContainer

T = TypeVar('T')


class PbsIniData(Generic[T], abc.ABC):
    def __init__(self, config_path: str, args: T):
        ini_data = IniData(config_path)
        self.__data = []
        schema = self.get_schema(ini_data, args)
        for section_name, data in ini_data:
            self._preprocess_data(section_name, data)
            item = convert_data_to_json(section_name, data, schema)
            self._fix_data(item, schema)
            self.__data.append(item)

    @abc.abstractmethod
    def _preprocess_data(self, section_name: str, data: dict[str, str]):
        pass

    @abc.abstractmethod
    def get_schema(self, ini_data: IniData, args: T) -> dict[str, tuple[str, str, Optional[DataContainer]]]:
        pass

    @abc.abstractmethod
    def _fix_data(self, item: dict[str, any], schema: dict[str, tuple[str, str, Optional[set[str]]]]):
        pass

    @staticmethod
    def _get_key(item: dict[str, any]) -> str:
        return item["Name"]

    def get_keys(self) -> set[str]:
        return set(map(self._get_key, self.__data))

    def to_json(self) -> str:
        return json.dumps(self.__data)
