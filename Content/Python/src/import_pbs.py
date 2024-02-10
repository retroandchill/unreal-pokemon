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
import os

from unreal import Paths, FieldUse, BattleUse, MoveDamageCategory

from pokemon.data_loader import UnrealDataLoader
from pokemon.data_loader.pbs_data import ItemData, MoveData, TypeData, AbilityData, SpeciesData
from pokemon.data_writer import import_items, import_moves, import_types, import_abilities
from pokemon.data_writer.data_table_writer import import_species
from pokemon.unreal_interface import enum_values, data_table_values
from pokemon.unreal_interface.unreal_data_utils import stat_entries

HARDCODED_DATA_DIR = "Data/Hardcoded"

if __name__ == "__main__":
    base_path = Paths.project_dir()
    data_dir = "/Game/Content/Data"
    pbs_dir = os.path.join(base_path, "PBS")

    types = TypeData(os.path.join(pbs_dir, "types.txt"))
    import_types(types)

    type_ids = UnrealDataLoader(types.get_keys)
    damage_category_enum = enum_values(MoveDamageCategory)
    target_ids = data_table_values(HARDCODED_DATA_DIR, "Targets")
    moves = MoveData(os.path.join(pbs_dir, "moves.txt"), type_ids, damage_category_enum, target_ids)
    import_moves(moves)

    field_use_enum = enum_values(FieldUse)
    battle_use_enum = enum_values(BattleUse)
    move_ids = UnrealDataLoader(moves.get_keys)
    items = ItemData(os.path.join(pbs_dir, "items.txt"), field_use_enum, battle_use_enum, move_ids)
    import_items(items)

    abilities = AbilityData(os.path.join(pbs_dir, "abilities.txt"))
    import_abilities(abilities)

    gender_ratio_ids = data_table_values(HARDCODED_DATA_DIR, "GenderRatios")
    growth_rate_ids = data_table_values(HARDCODED_DATA_DIR, "GrowthRates")
    stats = stat_entries(HARDCODED_DATA_DIR, "Stats")
    ability_ids = UnrealDataLoader(abilities.get_keys)
    egg_group_ids = data_table_values(HARDCODED_DATA_DIR, "EggGroups")
    item_ids = UnrealDataLoader(items.get_keys)
    body_color_ids = data_table_values(HARDCODED_DATA_DIR, "BodyColors")
    body_shape_ids = data_table_values(HARDCODED_DATA_DIR, "BodyShapes")
    habitat_ids = data_table_values(HARDCODED_DATA_DIR, "Habitats")
    evolution_ids = data_table_values(HARDCODED_DATA_DIR, "Evolutions")
    species = SpeciesData(os.path.join(pbs_dir, "pokemon.txt"), type_ids, gender_ratio_ids, growth_rate_ids, stats,
                          ability_ids, move_ids, egg_group_ids, item_ids, body_color_ids, body_shape_ids, habitat_ids,
                          evolution_ids)
    import_species(species)
