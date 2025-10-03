using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping functionalities for converting between different representations
/// of trainer type data, specifically between <see cref="FTrainerType"/> and <see cref="TrainerTypeInfo"/>.
/// </summary>
/// <remarks>
/// This mapper is designed to facilitate the creation, transformation, and synchronization
/// of trainer type details across different layers or contexts within the application.
/// It uses the Mapperly library to generate mapping code for specified properties.
/// </remarks>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class TrainerTypeMapper
{
    /// <summary>
    /// Maps a <see cref="TrainerTypeInfo"/> instance to an <see cref="FTrainerType"/> instance.
    /// </summary>
    /// <param name="abilityInfo">The source <see cref="TrainerTypeInfo"/> object containing trainer type details to map from.</param>
    /// <returns>An <see cref="FTrainerType"/> object representing the mapped trainer type details.</returns>
    [MapPropertyFromSource(nameof(FTrainerType.SkillLevel), Use = nameof(MapSkillLevel))]
    public static partial FTrainerType ToTrainerType(this TrainerTypeInfo abilityInfo);

    /// <summary>
    /// Maps an <see cref="FTrainerType"/> instance to a <see cref="TrainerTypeInfo"/> instance.
    /// </summary>
    /// <param name="ability">The source <see cref="FTrainerType"/> object containing trainer type details to map from.</param>
    /// <returns>A <see cref="TrainerTypeInfo"/> object representing the mapped trainer type details.</returns>
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
