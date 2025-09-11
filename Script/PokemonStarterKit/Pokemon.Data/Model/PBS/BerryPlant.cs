using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

[UStruct]
[CreateStructView]
public readonly partial struct FBerryPlant() : IGameDataEntry
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
        Category = "Growth"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int HoursPerStage { get; init; } = 3;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Growth"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int DryingPerHour { get; init; } = 15;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Growth"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    [AsValue]
    public FInt32Range Yield { get; init; }

    public int MinimumYield => Yield.LowerBound.Value;

    public int MaximumYield => Yield.UpperBound.Value;
}

[UClass]
[GameDataRepository<FBerryPlant>]
[UsedImplicitly]
public partial class UBerryPlantRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(
    typeof(GameData),
    nameof(GameData.BerryPlants),
    ComparableTypes = [typeof(FItemHandle)]
)]
public readonly partial record struct FBerryPlantHandle;
