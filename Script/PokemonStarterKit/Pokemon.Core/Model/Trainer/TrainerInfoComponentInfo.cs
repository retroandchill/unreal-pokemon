using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Model.Trainer;

[UStruct]
public readonly partial record struct FTrainerInfoComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FTrainerTypeHandle TrainerType { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText Name { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required uint FullID { get; init; }
}
