using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Moves;

[UStruct]
public readonly partial record struct FPokemonMoveInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FMoveHandle Move { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int PP { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int PPUps { get; init; }
}
