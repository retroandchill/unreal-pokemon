namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Defines the capability for an object to be healable.
/// Commonly implemented by components that require restoration of status,
/// health, or other attributes to their optimal state.
/// </summary>
public interface IHealableComponent
{
    /// <summary>
    /// Restores the object to its optimal state by healing its attributes.
    /// The specific implementation varies depending on the type of component
    /// invoking the method. Healing may reset health, remove status effects, or
    /// perform other restorative actions based on the component.
    /// </summary>
    void Heal();
}
