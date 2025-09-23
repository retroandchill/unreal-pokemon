using LanguageExt;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FStatComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int Level { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int Exp { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required int HP { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> IV { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> IVOverrides { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyDictionary<FMainStatHandle, int> EV { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required FNatureHandle Nature { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FNatureHandle> NatureOverride { get; init; }
}
