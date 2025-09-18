using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public record TrainerTypeInfo
{
    [PbsKey]
    public FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsLocalizedText("PokemonTrainerType", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    public ETrainerGender Gender { get; init; } = ETrainerGender.Unknown;

    [PbsRange<int>(0)]
    public int BaseMoney { get; init; } = 30;

    [PbsRange<int>(0)]
    public int? SkillLevel { get; init; }

    [PbsName("Flags")]
    [PbsGameplayTag("Data.TrainerType", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }
}
