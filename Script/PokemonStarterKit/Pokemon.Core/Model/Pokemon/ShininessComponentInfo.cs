using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FShininessComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required bool IsShiny { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required bool IsSuperShiny { get; init; }
}
