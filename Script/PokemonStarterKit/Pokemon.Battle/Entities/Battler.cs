using Pokemon.Battle.Components;
using Pokemon.Battle.Components.Battler;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Entities;

/// <summary>
/// Represents the core battler entity in a Pokémon battle, extending the functionalities of an RPG entity.
/// </summary>
/// <remarks>
/// This class serves as an abstract base class for battlers in the battle system. It contains various
/// components that handle different aspects of a battler such as information, stats, abilities, hold items,
/// status effects, battle effects, and battle context.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public partial class UBattler : URPGEntity, IEffectEntity
{
    /// <summary>
    /// Represents the information component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property is part of the UBattler class and provides detailed information about the battler.
    /// It is initialized as an instance within the UBattler class and categorized under "Components".
    /// This component is marked as read-only and can only be set privately, ensuring controlled access.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerInfoComponent InfoComponent { get; private set; }

    /// <summary>
    /// Represents the statistics component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property is part of the UBattler class and handles stats-related data and functionality for the battler.
    /// It is initialized as an instance within the UBattler class and categorized under "Components".
    /// This component is read-only and can only be set privately, ensuring controlled and consistent stat management.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerStatComponent StatComponent { get; private set; }

    /// <summary>
    /// Represents the ability component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property belongs to the UBattler class and is responsible for managing the abilities of the battler.
    /// It is initialized as an instance within the UBattler class and categorized under "Components".
    /// This component is read-only and provides controlled access, with private set capabilities.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerAbilityComponent AbilityComponent { get; private set; }

    /// <summary>
    /// Represents the hold item component associated with a battler entity.
    /// </summary>
    /// <remarks>
    /// This property is part of the UBattler class and manages the item held by the battler during battles.
    /// It is defined as an instance within the UBattler class and belongs to the "Components" category.
    /// The component is read-only, ensuring its value can only be modified internally.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerHoldItemComponent HoldItemComponent { get; private set; }

    /// <summary>
    /// Represents the status effect component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property is part of the UBattler class and is responsible for managing the status effects applied to the battler.
    /// It is initialized as an instance within the UBattler class under the "Components" category.
    /// This component is marked as read-only and can only be set privately, maintaining controlled access during runtime.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerStatusEffectComponent StatusEffectComponent { get; private set; }

    /// <summary>
    /// Represents the effect component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property belongs to the UBattler class and provides an interface for managing battle-related effects specific to the battler.
    /// It is an instance of the UBattleEffectComponent class and categorized under "Components".
    /// The property is initialized privately and is marked as read-only, ensuring encapsulated modification and external access for retrieval only.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattleEffectComponent EffectComponent { get; private set; }

    /// <summary>
    /// Represents the contextual component of a battler entity.
    /// </summary>
    /// <remarks>
    /// This property belongs to the UBattler class and provides dynamic context-related functionality for the battler during a battle.
    /// It is initialized as an instance within the UBattler class and categorized under "Components".
    /// This component is read-only, allowing private setting to ensure controlled initialization and data integrity.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerContextComponent ContextComponent { get; private set; }
}
