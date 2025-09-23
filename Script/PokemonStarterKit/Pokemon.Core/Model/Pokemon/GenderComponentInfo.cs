using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

[UStruct]
public readonly partial record struct FGenderComponentInfo
{
    [field: UProperty(PropertyFlags.SaveGame)]
    public required EPokemonGender Gender { get; init; }
}
