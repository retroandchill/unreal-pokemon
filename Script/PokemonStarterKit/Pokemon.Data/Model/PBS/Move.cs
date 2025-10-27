using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;
using UnrealSharp.GameplayTags;
using ZLinq;

namespace Pokemon.Data.Model.PBS;

/// <summary>
/// Specifies the category of damage a move inflicts in a Pokémon game.
/// </summary>
[UEnum]
public enum EDamageCategory : byte
{
    /// <summary>
    /// Damage is calculated using the attacker's Attack stat and the defender's Defense stat.
    /// </summary>
    Physical,

    /// <summary>
    /// Damage is calculated using the attacker's Special Attack stat and the defender's Special Defense stat.
    /// </summary>
    Special,

    /// <summary>
    /// Non-damaging moves, typically used to cause special effects or alter stats.
    /// </summary>
    Status,
}

/// <summary>
/// Represents the types of damage calculations available for a move in a Pokémon game.
/// </summary>
[UEnum]
public enum EDamageType : byte
{
    /// <summary>
    /// Move does not inflict damage.
    /// </summary>
    NoDamage,

    /// <summary>
    /// Move has a fixed power.
    /// </summary>
    FixedPower,

    /// <summary>
    /// Move power is variable.
    /// </summary>
    VariablePower,
}

/// <summary>
/// A structure that represents detailed information about the damage type for a Pokémon move.
/// </summary>
/// <remarks>
/// Used to determine how damage is calculated for a specific move within the game context.
/// Allows differentiation between fixed power, variable power, or no damage moves.
/// </remarks>
[UStruct]
public readonly partial struct FDamageTypeData
{
    private readonly EDamageType _damageType;
    private readonly int _power;

    /// <summary>
    /// A structure that represents detailed information about the damage type for a Pokémon move.
    /// </summary>
    /// <remarks>
    /// Used to determine how damage is calculated for a specific move within the game context.
    /// Allows differentiation between fixed power, variable power, or no damage moves.
    /// </remarks>
    public FDamageTypeData(EDamageType damageType, int power)
    {
        _damageType = damageType;
        _power = power;
    }

    /// <summary>
    /// Executes one of the provided actions based on the damage type of the current instance.
    /// </summary>
    /// <param name="onFixedPower">Action executed when the damage type is fixed power, with the damage power provided.</param>
    /// <param name="onVariablePower">Action executed when the damage type is variable power.</param>
    /// <param name="onNoDamage">Action executed when there is no damage type.</param>
    /// <exception cref="InvalidOperationException">Thrown when the damage type is invalid or unsupported.</exception>
    public void Match(Action<int> onFixedPower, Action onVariablePower, Action onNoDamage)
    {
        switch (_damageType)
        {
            case EDamageType.NoDamage:
                onNoDamage();
                break;
            case EDamageType.FixedPower:
                onFixedPower(_power);
                break;
            case EDamageType.VariablePower:
                onVariablePower();
                break;
            default:
                throw new InvalidOperationException("Invalid damage type");
        }
    }

    /// <summary>
    /// Executes one of the provided functions based on the damage type of the current instance and returns a result of the specified type.
    /// </summary>
    /// <typeparam name="T">The type of the return value from the provided functions.</typeparam>
    /// <param name="onFixedPower">Function executed when the damage type is fixed power, with the damage power provided.</param>
    /// <param name="onVariablePower">Function executed when the damage type is variable power.</param>
    /// <param name="onNoDamage">Function executed when there is no damage type.</param>
    /// <returns>The result of the invoked function of type <typeparamref name="T"/>.
    /// </returns>
    /// <exception cref="InvalidOperationException">Thrown when the damage type is invalid or unsupported.</exception>
    public T Match<T>(Func<int, T> onFixedPower, Func<T> onVariablePower, Func<T> onNoDamage)
    {
        return _damageType switch
        {
            EDamageType.NoDamage => onNoDamage(),
            EDamageType.FixedPower => onFixedPower(_power),
            EDamageType.VariablePower => onVariablePower(),
            _ => throw new InvalidOperationException("Invalid damage type"),
        };
    }
}

/// <summary>
/// A structure that represents the detailed data and properties of a Pokémon move.
/// </summary>
/// <remarks>
/// This structure holds critical information about a Pokémon move, including its
/// identification, type classification, power, accuracy, and special effects.
/// It defines how the move behaves during gameplay, its battle effects, and its
/// associated metadata.
/// </remarks>
[UStruct]
[CreateStructView]
public readonly partial struct FMove() : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Identification")]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.VisibleAnywhere, Category = "Identification")]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the display name of the move.
    /// This property is read-only and is intended for use in display contexts within the game.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    [GameAccessTools.SourceGenerator.Attributes.DisplayName]
    public FText Name { get; init; } = "Unnamed";

    /// <summary>
    /// Represents the description of the move, providing detailed information displayed in the context of the game.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Display")]
    public FText Description { get; init; } = "???";

    /// <summary>
    /// Represents the elemental type or classification of a move in the Pokémon game data.
    /// This property determines attributes and interactions specific to the type,
    /// such as whether the move is physical or special.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Classification")]
    [AsValue]
    public FTypeHandle Type { get; init; }

    /// <summary>
    /// Represents the damage category of the move, such as Physical, Special, or Status.
    /// This property determines how damage or effects are calculated during gameplay.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Classification")]
    public EDamageCategory Category { get; init; } = EDamageCategory.Status;

    /// <summary>
    /// Represents the type of damage inflicted by the move.
    /// Determines how the move's power or damage output is calculated.
    /// Possible values include no damage, fixed power, or variable power.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    public EDamageType DamageType { get; init; }

    /// <summary>
    /// Represents the power value of a move, typically used when the damage type is set to FixedPower.
    /// The value must be greater than or equal to 5 and is controlled by a clamp to ensure valid input.
    /// This property is conditionally editable based on the DamageType being FixedPower.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    [field: ClampMin("5")]
    [field: UIMin("5")]
    [field: EditCondition($"{nameof(DamageType)} == DamageType::{nameof(EDamageType.FixedPower)}")]
    [field: EditConditionHides]
    public int Power { get; init; } = 5;

    /// <summary>
    /// Indicates whether the move is guaranteed to always hit its target, bypassing accuracy checks.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    public bool AlwaysHits { get; init; }

    /// <summary>
    /// Represents the accuracy percentage of the move, determining the likelihood of it successfully hitting its target.
    /// </summary>
    /// <remarks>
    /// The value is clamped between 1 and 100. Accuracy is only applicable when <c>AlwaysHits</c> is set to <c>false</c>.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    [field: ClampMin("1")]
    [field: ClampMax("100")]
    [field: UIMin("1")]
    [field: UIMax("100")]
    [field: EditCondition($"!{nameof(AlwaysHits)}")]
    public int Accuracy { get; init; } = 100;

    /// <summary>
    /// Represents the total number of Power Points (PP) that a move has.
    /// This value determines the maximum number of times the move can be used.
    /// The value is clamped to a minimum of 1 to ensure validity.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Stats")]
    [field: ClampMin("1")]
    [field: UIMin("1")]
    public int TotalPP { get; init; } = 5;

    /// <summary>
    /// Gets the priority of the move, which determines the order of execution during a battle.
    /// Positive values indicate a higher priority, while negative values indicate a lower priority.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "BattleUsage")]
    public int Priority { get; init; } = 0;

    /// <summary>
    /// Represents the target associated with a specific move in the battle system.
    /// This property provides information about the target definition of the move,
    /// enabling retrieval of target-related details for gameplay logic.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "BattleUsage")]
    [AsValue]
    public FTargetHandle Target { get; init; }

    /// <summary>
    /// Gets the function code associated with the move, defining its core behavior or effect within the game.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Effect")]
    [Categories(IdentifierConstants.FunctionCodeTag)]
    [AsValue]
    public FGameplayTag FunctionCode { get; init; }

    /// <summary>
    /// Indicates whether the effect of the move is guaranteed to occur.
    /// If true, the effect always takes place; otherwise, its occurrence is determined by the EffectChance property.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Effect")]
    public bool GuaranteedEffect { get; init; } = true;

    /// <summary>
    /// Represents the percentage chance for an effect to occur when the move is used.
    /// The value is an integer between 1 and 100 and is only applicable if the move does not have a guaranteed effect.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Effect")]
    [field: ClampMin("1")]
    [field: ClampMax("100")]
    [field: UIMin("1")]
    [field: UIMax("100")]
    [field: EditCondition($"!{nameof(GuaranteedEffect)}")]
    [field: EditConditionHides]
    public int EffectChance { get; init; } = 30;

    /// <summary>
    /// Represents a container for gameplay tags associated with the move.
    /// These tags can be used in various game systems to identify and categorize the move.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly | PropertyFlags.EditAnywhere, Category = "Metadata")]
    public FGameplayTagContainer Tags { get; init; }

    /// <summary>
    /// Represents the damage type and power properties of the move.
    /// Combines the `DamageType` and `Power` values into a single `FDamageTypeData` structure.
    /// </summary>
    [AsValue]
    public FDamageTypeData Damage => new(DamageType, Power);

    /// <summary>
    /// Indicates whether the move is classified as a physical attack.
    /// A move is considered physical if its damage type is not `NoDamage`
    /// and meets specific conditions based on the game's settings.
    /// When the `MoveCategoryPerMove` setting is enabled, the type's physical characteristic is used to determine this,
    /// otherwise it depends on the move's damage category.
    /// </summary>
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

    /// <summary>
    /// Indicates whether the move is classified as "special," based on its damage type and other classification rules.
    /// A move is considered special if it deals damage and either the default settings allow individual move categories
    /// to determine this, or the move's type or category corresponds to the special classification.
    /// Returns:
    /// True if the move is classified as special; otherwise, false.
    /// </summary>
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

    /// <summary>
    /// Indicates whether the move deals damage.
    /// Returns true if the move's category is either Physical or Special; otherwise, false.
    /// </summary>
    public bool IsDamaging => Category != EDamageCategory.Status;

    /// <summary>
    /// Indicates whether the move belongs to the "Status" damage category.
    /// This property evaluates the `Category` field of the move to determine if it is classified as `EDamageCategory.Status`.
    /// </summary>
    public bool IsStatus => Category == EDamageCategory.Status;

    /// <summary>
    /// Indicates whether the move is classified as a Hidden Move (HM).
    /// This determination is based on whether the move is associated with any game
    /// data entries marked as Hidden Moves.
    /// </summary>
    public bool IsHiddenMove
    {
        get
        {
            var id = ID;
            return GameData.Items.Entries.AsValueEnumerable(i => (i.Move, i.IsHM)).Any(x => x.IsHM && x.Move == id);
        }
    }
}

/// <summary>
/// Repository class responsible for managing and providing access to move-related game data.
/// </summary>
/// <remarks>
/// This class acts as a specialized repository for handling instances of <c>FMove</c>,
/// enabling operations such as retrieval and management within the game's data infrastructure.
/// Utilized by the <c>GameData</c> system to dynamically fetch and access Pokémon moves.
/// </remarks>
[UClass]
[GameDataRepository<FMove>]
public partial class UMoveRepository : UAssetGameDataRepository;

/// <summary>
/// A structured handle representing a reference to a Pokémon move within the game data system.
/// </summary>
/// <remarks>
/// Used in the game data context to access and manipulate move definitions efficiently and reliably.
/// This struct acts as an identifier or pointer linking to detailed move metadata contained in the game's data repository.
/// </remarks>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Moves))]
public readonly partial record struct FMoveHandle([property: UProperty(PropertyFlags.EditAnywhere)] FName ID);
