using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents the information associated with a trainer type in the Pokémon game system.
/// </summary>
/// <remarks>
/// This record is used to define various properties related to a trainer type, including
/// its identifier, gender, base money, associated tags, and other related data. It is
/// primarily utilized in the context of serialization and deserialization of trainer type
/// data within the Pokémon editor.
/// </remarks>
public record TrainerTypeInfo
{
    /// <summary>
    /// Gets the unique identifier for the trainer type.
    /// </summary>
    /// <remarks>
    /// Used as the primary key within the Pokémon editor serialization system to uniquely
    /// identify a specific trainer type. This property is marked with the <see cref="PbsKeyAttribute"/>
    /// to designate it as a critical field for serialization.
    /// </remarks>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Gets the index of the row associated with this trainer type in the PBS file.
    /// </summary>
    /// <remarks>
    /// The row index represents the position of the trainer type within the serialized PBS data.
    /// This property is marked with the <see cref="PbsIndexAttribute"/> to signify its role
    /// as the indexed position used during serialization processes.
    /// </remarks>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the localized display name for the trainer type.
    /// </summary>
    /// <remarks>
    /// This property holds the user-facing name of the trainer type, which is stored as a localized text value.
    /// The display name is retrieved using the <see cref="PbsLocalizedTextAttribute"/> with specific namespace
    /// and key formatting for localization purposes.
    /// </remarks>
    [PbsLocalizedText("PokemonTrainerType", "{0}_DisplayName")]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Gets the gender type associated with the trainer.
    /// </summary>
    /// <remarks>
    /// Indicates the gender classification of a trainer, which is represented
    /// by the <see cref="ETrainerGender"/> enumeration. This property can be
    /// set to specific gender values such as Male, Female, Unknown, or Mixed
    /// depending on the trainer's characteristics.
    /// </remarks>
    public ETrainerGender Gender { get; init; } = ETrainerGender.Unknown;

    /// <summary>
    /// Gets the base monetary reward for defeating a trainer of this type.
    /// </summary>
    /// <remarks>
    /// Defines the minimum amount of in-game currency awarded to the player upon victory against
    /// a trainer of this type. This value is constrained by the <see cref="PbsRangeAttribute{T}"/>
    /// to ensure it is non-negative.
    /// </remarks>
    [PbsRange<int>(0)]
    public int BaseMoney { get; init; } = 30;

    /// <summary>
    /// Gets the skill level of the trainer type.
    /// </summary>
    /// <remarks>
    /// Represents an optional value indicating the skill level associated with a trainer type.
    /// If not explicitly specified, this value may default to another property, such as <see cref="BaseMoney"/>,
    /// during mapping or processing. This property is constrained by the <see cref="PbsRangeAttribute{T}"/> to ensure it meets
    /// a minimum value of 0.
    /// </remarks>
    [PbsRange<int>(0)]
    public int? SkillLevel { get; init; }

    /// <summary>
    /// Gets the collection of gameplay tags associated with the trainer type.
    /// </summary>
    /// <remarks>
    /// Tags categorized under the "Data.TrainerType" namespace represent various metadata or
    /// characteristics associated with the trainer type. This property is annotated with the
    /// <see cref="PbsNameAttribute"/> with the name "Flags" and <see cref="PbsGameplayTagAttribute"/>
    /// to denote its structured usage within the gameplay tag system. The tags are automatically
    /// created if they don't exist, with an underscore (_) used as the separator for hierarchical tags.
    /// </remarks>
    [PbsName("Flags")]
    [PbsGameplayTag("Data.TrainerType", Create = true, Separator = "_")]
    public FGameplayTagContainer Tags { get; init; }
}
