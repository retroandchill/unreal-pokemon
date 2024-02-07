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
import re
from typing import Type

from unreal import EnumBase, Name, EditorAssetLibrary, DataTable, DataTableFunctionLibrary


def enum_values(enum: Type[EnumBase]) -> set[str]:
    ret = set()
    pattern = re.compile(r'\s+')
    for attr in vars(enum):
        value = getattr(enum, attr)
        if isinstance(value, enum):
            display_name = str(value.get_display_name())
            ret.add(re.sub(pattern, '', display_name))

    return ret


def convert_name(name: Name) -> str:
    return str(name)


def data_table_values(directory_name: str, table_name: str) -> set[str]:
    eal = EditorAssetLibrary()
    data_table = eal.load_asset('/Game/{0}/{1}.{1}'.format(directory_name, table_name))
    if isinstance(data_table, DataTable):
        return set(map(convert_name, DataTableFunctionLibrary.get_data_table_row_names(data_table)))
    else:
        raise ValueError("Unexpected data table name")
