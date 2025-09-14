using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

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

[UStruct]
[CreateStructView]
public readonly partial struct FTrainerType() : IGameDataEntry
{
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public FName ID { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Profile"
    )]
    public ETrainerGender Gender { get; init; } = ETrainerGender.Unknown;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Battle"
    )]
    [field: UIMin("0")]
    [field: ClampMin("0")]
    public int BaseMoney { get; init; } = 30;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Battle"
    )]
    [field: UIMin("0")]
    [field: ClampMin("0")]
    public int SkillLevel { get; init; } = 30;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }
}

[UClass]
[GameDataRepository<FTrainerType>]
[UsedImplicitly]
public partial class UTrainerTypeRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.TrainerTypes))]
public readonly partial record struct FTrainerTypeHandle;
