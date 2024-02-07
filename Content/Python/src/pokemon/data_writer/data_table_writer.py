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

from pokemon.data_loader.pbs_data import PbsIniData, ItemData, MoveData, TypeData


def import_data(item_data: PbsIniData, table_name: str) -> None:
    """
    Import data from the given PBS data and insert it into a DataTable asset
    :param item_data: The PBS data to import
    :param table_name: The name of the data table as seen in the Data folder of the Content browser
    """
    eal = EditorAssetLibrary()
    data_table = eal.load_asset('/Game/Data/{0}.{0}'.format(table_name))
    if isinstance(data_table, DataTable):
        DataTableFunctionLibrary.fill_data_table_from_json_string(data_table, item_data.to_json())


def import_types(type_data: TypeData) -> None:
    """
    Import type data into Unreal
    :param type_data: The list of types to import
    """
    print("Importing moves...")
    import_data(type_data, "Types")


def import_moves(move_data: MoveData) -> None:
    """
    Import move data into Unreal
    :param move_data: The list of moves to import
    """
    print("Importing moves...")
    import_data(move_data, "Moves")


def import_items(item_data: ItemData) -> None:
    """
    Import item data into Unreal
    :param item_data: The list of items to import
    """
    print("Importing items...")
    import_data(item_data, "Items")
