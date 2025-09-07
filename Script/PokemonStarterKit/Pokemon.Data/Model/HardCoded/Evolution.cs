using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
public readonly record struct FIntEvolutionConditionData
{
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public int Parameter { get; init; }
}

[UStruct]
public readonly partial record struct FMoveEvolutionConditionData
{
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Move ID"
    )]
    public FGameplayTag MoveId { get; init; }
}

[UStruct]
public readonly record struct FTypeEvolutionConditionData
{
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Type ID"
    )]
    public FGameplayTag TypeId { get; init; }
}

[UStruct]
public readonly record struct FItemEvolutionConditionData
{
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Item ID"
    )]
    public FGameplayTag ItemId { get; init; }
}

[UStruct]
public readonly record struct FSpeciesEvolutionConditionData
{
    [UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        DisplayName = "Species ID"
    )]
    public FGameplayTag SpeciesId { get; init; }
}

[UStruct]
public readonly record struct FLocationEvolutionConditionData
{
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    public TSoftObjectPtr<UWorld> Level { get; init; }
}

[UStruct]
public readonly record struct FLocationFlagEvolutionConditionData
{
    public const string TagCategory = "Field.Location.Evolution";

    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere)]
    [Categories(TagCategory)]
    public FGameplayTag Flag { get; init; }
}

[UStruct]
[CreateStructView]
public readonly partial struct FEvolution : IGameDataEntry
{
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public required FText Name { get; init; }

    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required UScriptStruct? Parameters { get; init; }
}

[UClass]
[GameDataRepository<FEvolution>]
[UsedImplicitly]
public partial class UEvolutionRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Evolutions))]
public readonly partial record struct FEvolutionHandle;

public static class EvolutionExtensions
{
    private const string LocalizationNamespace = "GameData.Evolution";

    public static UGameDataManager AddEvolutions(this UGameDataManager manager)
    {
        return manager;
    }
}
