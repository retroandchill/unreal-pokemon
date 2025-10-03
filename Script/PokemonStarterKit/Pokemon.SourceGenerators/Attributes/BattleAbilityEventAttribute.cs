#if POKEMON_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace Pokemon.SourceGenerators.Attributes;

/// <summary>
/// An attribute used to mark methods that are listeners for specific battle ability events.
/// This attribute provides metadata about the event name and associated ability identifier for the method.
/// </summary>
/// <remarks>
/// The attribute is intended to be applied to methods only and supports source generation
/// tools to identify and handle event listeners during compile time.
/// </remarks>
/// <param name="eventName">The name of the battle ability event that the method responds to.</param>
/// <param name="abilityId">The unique identifier of the ability associated with the event.</param>
[AttributeUsage(AttributeTargets.Method)]
#if POKEMON_GENERATOR
[IncludeFile]
#endif
internal class BattleAbilityEventAttribute(string eventName, string abilityId) : Attribute
{
    /// <summary>
    /// Gets the name of the battle ability event.
    /// </summary>
    /// <remarks>
    /// The event name uniquely identifies the specific event that the attributed method responds to.
    /// This property is set via the constructor of the <see cref="BattleAbilityEventAttribute"/> class.
    /// </remarks>
    public string EventName { get; } = eventName;

    /// <summary>
    /// Gets the unique identifier of the ability associated with the event.
    /// </summary>
    /// <remarks>
    /// The ability ID serves as a reference to the specific ability that the attributed
    /// method is related to, enabling precise mapping of methods to abilities for event handling.
    /// This property is set via the constructor of the <see cref="BattleAbilityEventAttribute"/> class.
    /// </remarks>
    public string AbilityID { get; } = abilityId;
}
