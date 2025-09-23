using LanguageExt;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FOwnershipComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int ID { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int SecretID { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText OTName { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required ETrainerGender OTGender { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required EObtainMethod ObtainMethod { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int LevelMet { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FDateTime TimeReceived { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText ObtainText { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FHatchInformation> HatchInformation { get; init; }
}
