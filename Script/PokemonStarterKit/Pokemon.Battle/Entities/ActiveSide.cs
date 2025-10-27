using Pokemon.Battle.Components;
using Pokemon.Battle.Components.Side;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Entities;

/// <summary>
/// Represents an active side within the context of a battle.
/// </summary>
/// <remarks>
/// The <c>UActiveSide</c> class inherits from <see cref="URPGEntity"/> and implements the <see cref="IEffectEntity"/> interface.
/// It is designed to encapsulate the behavior and state of an active side during a battle, providing
/// functionality and context through its associated components.
/// </remarks>
[UClass]
public partial class UActiveSide : URPGEntity, IEffectEntity
{
    /// <summary>
    /// Represents the battle effect component associated with an active side or other battle entities.
    /// </summary>
    /// <remarks>
    /// The <c>EffectComponent</c> property provides access to the <c>UBattleEffectComponent</c> instance,
    /// which manages and applies various effects within the context of a Pokémon battle.
    /// This property is part of the <c>UActiveSide</c> class and is integral to handling battle-related
    /// logic and interactions.
    /// </remarks>
    /// <value>
    /// A <c>UBattleEffectComponent</c> instance that encapsulates functionality for managing effects
    /// on a battle entity or active side.
    /// </value>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UBattleEffectComponent EffectComponent { get; private set; }

    /// <summary>
    /// Represents the context component associated with an active side in a Pokémon battle.
    /// </summary>
    /// <remarks>
    /// The <c>ContextComponent</c> property provides access to the <c>UActiveSideContextComponent</c> instance,
    /// which manages the contextual relationships and lifecycle interactions between an active side
    /// and the battle environment it participates in. This property is part of the <c>UActiveSide</c> class
    /// and is crucial for transient battle context handling.
    /// </remarks>
    /// <value>
    /// A <c>UActiveSideContextComponent</c> instance that encapsulates the functionality required to manage
    /// the context related to a battle's active side.
    /// </value>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UActiveSideContextComponent ContextComponent { get; private set; }
}
