using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;

namespace Pokemon.Data.Model.PBS;

[UEnum]
public enum EDamageCategory : byte
{
    Physical,
    Special,
    Status,
}

[UEnum]
public enum EDamageType : byte
{
    NoDamage,
    FixedPower,
    VariablePower,
}

[UStruct]
[CreateStructView]
public readonly partial struct FMove() : IGameDataEntry
{
    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

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
        Category = "Display"
    )]
    public FText Description { get; init; } = "???";

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Classification"
    )]
    [AsValue]
    public FTypeHandle Type { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Classification"
    )]
    public EDamageCategory Category { get; init; } = EDamageCategory.Status;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    public EDamageType DamageType { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    [field: ClampMin("5")]
    [field: UIMin("5")]
    [field: EditCondition($"{nameof(DamageType)} == DamageType::{nameof(EDamageType.FixedPower)}")]
    [field: EditConditionHides]
    public int Power { get; init; } = 5;

    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    public bool AlwaysHits { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    [field: ClampMin("1")]
    [field: ClampMax("100")]
    [field: UIMin("1")]
    [field: UIMax("100")]
    [field: EditCondition($"!{nameof(AlwaysHits)}")]
    public int Accuracy { get; init; } = 100;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Stats"
    )]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int TotalPP { get; init; } = 5;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "BattleUsage"
    )]
    public int Priority { get; init; } = 0;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "BattleUsage"
    )]
    [AsValue]
    public FTargetHandle Target { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Effect"
    )]
    [Categories(IdentifierConstants.FunctionCodeTag)]
    public FGameplayTag FunctionCode { get; init; }

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Effect"
    )]
    public bool GuaranteedEffect { get; init; } = true;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Effect"
    )]
    [field: ClampMin("1")]
    [field: ClampMax("100")]
    [field: UIMin("1")]
    [field: UIMax("100")]
    [field: EditCondition($"!{nameof(GuaranteedEffect)}")]
    [field: EditConditionHides]
    public int EffectChance { get; init; } = 30;

    [field: UProperty(
        PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere,
        Category = "Metadata"
    )]
    public FGameplayTagContainer Tags { get; init; }

    public bool IsPhysical
    {
        get
        {
            if (DamageType == EDamageType.NoDamage)
            {
                return false;
            }

            if (UObject.GetDefault<UPokemonDataSettings>().MoveCategoryPerMove)
            {
                return Type.Entry.IsPhysicalType;
            }

            return Category == EDamageCategory.Physical;
        }
    }

    public bool IsSpecial
    {
        get
        {
            if (DamageType == EDamageType.NoDamage)
            {
                return false;
            }

            if (UObject.GetDefault<UPokemonDataSettings>().MoveCategoryPerMove)
            {
                return Type.Entry.IsSpecialType;
            }

            return Category == EDamageCategory.Special;
        }
    }

    public bool IsDamaging => Category != EDamageCategory.Status;

    public bool IsStatus => Category == EDamageCategory.Status;
}

[UClass]
[GameDataRepository<FMove>]
[UsedImplicitly]
public partial class UMoveRepository : UAssetGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Moves))]
public readonly partial record struct FMoveHandle;

public record struct FakeEntry(bool IsSpecialType = true, bool IsPhysicalType = true);

public static class MoveExtensions
{
    extension(Type)
    {
        public static FakeEntry Entry => new FakeEntry();
    }
}
