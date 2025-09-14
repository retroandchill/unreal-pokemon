using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class TrainerTypeMapper
{
    [MapPropertyFromSource(nameof(FTrainerType.SkillLevel), Use = nameof(MapSkillLevel))]
    public static partial FTrainerType ToTrainerType(this TrainerTypeInfo abilityInfo);

    [MapPropertyFromSource(nameof(TrainerTypeInfo.SkillLevel), Use = nameof(MapNullableSkillLevel))]
    public static partial TrainerTypeInfo ToTrainerTypeInfo(this FTrainerType ability);

    private static int MapSkillLevel(this TrainerTypeInfo trainerType)
    {
        return trainerType.SkillLevel ?? trainerType.BaseMoney;
    }

    private static int? MapNullableSkillLevel(this FTrainerType trainerType)
    {
        return trainerType.SkillLevel != trainerType.BaseMoney ? trainerType.SkillLevel : null;
    }
}
