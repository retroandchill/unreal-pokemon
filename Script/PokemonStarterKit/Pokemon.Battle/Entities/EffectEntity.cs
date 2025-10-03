using Pokemon.Battle.Components;

namespace Pokemon.Battle.Entities;

/// <summary>
/// Defines an interface for entities that support battle effects.
/// </summary>
/// <remarks>
/// The <see cref="IEffectEntity"/> interface provides a contract for entities to integrate
/// with battle-related effects through the <see cref="UBattleEffectComponent"/>. Classes that
/// implement this interface can utilize the effect functionality encapsulated within the component.
/// </remarks>
public interface IEffectEntity
{
    /// <summary>
    /// Represents the component responsible for managing battle effects associated with an entity.
    /// This property is used in conjunction with the `IEffectEntity` interface to provide access
    /// to an entity's `UBattleEffectComponent`.
    /// The `EffectComponent` is typically used to apply, query, or manage various types of effects
    /// during a battle. It acts as a core component for handling effect-related logic within the
    /// battle system.
    /// </summary>
    UBattleEffectComponent EffectComponent { get; }
}
