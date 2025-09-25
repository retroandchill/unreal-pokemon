using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FIdentityComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FSpeciesHandle Species { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText Nickname { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required uint PersonalityValue { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required EPokemonGender Gender { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required bool IsShiny { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required bool IsSuperShiny { get; init; }
}
