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
from unreal import DataTable, DataTableFunctionLibrary, EditorAssetLibrary

from pokemon.data_loader import PbsIniData, ItemData, MoveData, TypeData


def import_data(item_data: PbsIniData, table_name: str):
    eal = EditorAssetLibrary()
    data_table = eal.load_asset('/Game/Data/{0}.{0}'.format(table_name))
    if isinstance(data_table, DataTable):
        DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json())


def import_types(type_data: TypeData):
    print("Importing moves...")
    import_data(type_data, "Types")


def import_moves(move_data: MoveData):
    print("Importing moves...")
    import_data(move_data, "Moves")


def import_items(item_data: ItemData):
    print("Importing items...")
    import_data(item_data, "Items")
