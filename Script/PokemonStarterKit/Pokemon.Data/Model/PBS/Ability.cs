using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using LanguageExt;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

[UStruct]
[CreateStructView]
public readonly partial struct FAbility() : IGameDataEntry
{
    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    [DisplayName]
    public FText Name { get; init; } = "Unnamed";

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Display"
    )]
    public FText Description { get; init; } = "???";

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }
}

[UClass]
[GameDataRepository<FAbility>]
[UsedImplicitly]
public partial class UAbilityRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Abilities))]
public readonly partial record struct FAbilityHandle;
