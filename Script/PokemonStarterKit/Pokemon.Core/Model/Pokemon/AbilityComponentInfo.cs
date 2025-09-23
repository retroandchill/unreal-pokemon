using LanguageExt;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FAbilityComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required int AbilityIndex { get; init; }

    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FAbilityHandle> ExplicitAbility { get; init; }
}
