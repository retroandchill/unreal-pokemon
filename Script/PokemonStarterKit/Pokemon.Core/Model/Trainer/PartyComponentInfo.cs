using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Model.Trainer;

[UStruct]
public readonly partial record struct FPartyComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FRPGEntitySaveDataHandle> Party { get; init; }
}
