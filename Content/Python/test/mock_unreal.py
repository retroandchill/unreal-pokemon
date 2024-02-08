class StringWrapper:
    def __init__(self, inner: str):
        self.inner = inner

    def __str__(self):
        return self.inner


Name = StringWrapper
Text = StringWrapper
PokemonStatType = StringWrapper


class Stat:
    r"""
    Represents one of the stats in the database

    **C++ Source:**

    - **Module**: PokemonData
    - **File**: Stat.h

    **Editor Properties:** (see get_editor_property/set_editor_property)

    - ``id`` (Name):  [Read-Write] The internal ID used for lookup by the game
    - ``pbs_order`` (int32):  [Read-Write] The order this stat is processed when parsing the PBS files
    - ``real_name`` (Text):  [Read-Write] The name that is displayed to the player
    - ``real_name_brief`` (Text):  [Read-Write] The name that is displayed to the player when there is a limited space
    - ``type`` (PokemonStatType):  [Read-Write] What kind of stat is this?
    """

    def __init__(self, id: Name, pbs_order: int, real_name: Text, real_name_brief: Text, type: PokemonStatType) -> None:
        self.id = id
        self.pbs_order = pbs_order
        self.real_name = real_name
        self.real_name_brief = real_name_brief
        self.type = type
