using LanguageExt;
using Pokemon.Core.Model.Moves;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FMoveComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FPokemonMoveInfo> Moves { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FMoveHandle> InitialMoves { get; init; }
}
