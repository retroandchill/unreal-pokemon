namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Represents a component that determines the battle capability of an entity or object.
/// </summary>
public interface IBattleCapableComponent
{
    /// <summary>
    /// Indicates whether the entity or object is currently capable of engaging in battle.
    /// </summary>
    /// <value>
    /// Returns <c>true</c> if the entity or object is able to battle; otherwise, <c>false</c>.
    /// </value>
    bool IsAbleToBattle { get; }
}
