using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

[UStruct]
[CreateStructView]
public readonly partial struct FType() : IGameDataEntry
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

    public bool IsPhysicalType => !IsSpecialType;

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Type Info"
    )]
    public bool IsSpecialType { get; init; } = false;

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Type Info"
    )]
    public bool IsPseudoType { get; init; } = false;

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Type Info"
    )]
    public IReadOnlyList<FTypeHandle> Weaknesses { get; init; } = [];

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Type Info"
    )]
    public IReadOnlyList<FTypeHandle> Resistances { get; init; } = [];

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Type Info"
    )]
    public IReadOnlyList<FTypeHandle> Immunities { get; init; } = [];

    [UsedImplicitly]
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }
}

[UClass]
[GameDataRepository<FType>]
[UsedImplicitly]
public partial class UTypeRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Types))]
public readonly partial record struct FTypeHandle;
