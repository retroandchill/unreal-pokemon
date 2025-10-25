using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Specifies the gender of a trainer.
/// </summary>
[UEnum]
public enum ETrainerGender : byte
{
    /// <summary>
    /// Represents a male trainer,
    /// </summary>
    Male = 0,

    /// <summary>
    /// Represents a female trainer
    /// </summary>
    Female = 1,

    /// <summary>
    /// Represents a trainer of unknown gender
    /// </summary>
    Unknown = 2,

    /// <summary>
    /// Represents a double battle trainer with a male and female member
    /// </summary>
    Mixed = 3,
}

/// <summary>
/// Represents a trainer type in the game, providing various properties for identification,
/// display, profile, and battle configuration.
/// </summary>
/// <remarks>
/// This struct serves as a data model for trainer types, used in contexts such as
/// defining trainer attributes, capabilities, and metadata within the game.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FTrainerType() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the display name associated with the trainer type. The value is localized and can be used for user-facing text.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Represents the gender of the trainer. This property uses the ETrainerGender enum
    /// to define possible values such as Male, Female, Unknown, or Mixed.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Profile")]
    public ETrainerGender Gender { get; init; } = ETrainerGender.Unknown;

    /// <summary>
    /// Represents the base amount of money associated with a trainer type.
    /// This value determines the financial resource typically attributed to a trainer
    /// during battles or other relevant gameplay scenarios.
    /// Must be a non-negative integer.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Battle")]
    [field: UIMin("0")]
    [field: ClampMin("0")]
    public int BaseMoney { get; init; } = 30;

    /// <summary>
    /// Represents the skill level of the trainer, determining their competency in battle scenarios.
    /// </summary>
    /// <remarks>
    /// This property is used to configure the relative difficulty or expertise of a trainer
    /// during battle interactions. It must be a non-negative integer value.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Battle")]
    [field: UIMin("0")]
    [field: ClampMin("0")]
    public int SkillLevel { get; init; } = 30;

    /// <summary>
    /// Represents a container for gameplay tags associated with the trainer type.
    /// Gameplay tags provide additional metadata or classification for a trainer type,
    /// allowing for contextual behaviors and attribute associations within the system.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }
}

/// <summary>
/// Represents a repository for managing and accessing trainer type data within the game.
/// </summary>
/// <remarks>
/// This class serves as a specialized repository derived from UAssetGameDataRepository, utilizing
/// FTrainerType as the data model for trainers and enabling efficient retrieval and handling of
/// trainer-related information.
/// </remarks>
[UClass]
[GameDataRepository<FTrainerType>]
[UsedImplicitly]
public partial class UTrainerTypeRepository : UAssetGameDataRepository;

/// <summary>
/// Represents a handle for referencing a trainer type entry within the game's data.
/// </summary>
/// <remarks>
/// This struct acts as a lightweight reference to a specific trainer type defined in the
/// game's data repository. It is used to ensure efficient access and identification of
/// trainer type entities without needing to duplicate the underlying data.
/// </remarks>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.TrainerTypes))]
public readonly partial record struct FTrainerTypeHandle;
