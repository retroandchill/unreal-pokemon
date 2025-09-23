using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FStatusEffectComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required Option<FStatusHandle> StatusEffect { get; init; }
}
