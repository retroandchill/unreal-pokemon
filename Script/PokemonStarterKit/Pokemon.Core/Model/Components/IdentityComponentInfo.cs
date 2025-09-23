using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Components;

[UStruct]
public readonly partial record struct FIdentityComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FSpeciesHandle Species { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText Nickname { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required uint PersonalityValue { get; init; }
}
