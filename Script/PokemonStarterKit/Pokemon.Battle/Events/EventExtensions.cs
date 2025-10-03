using Microsoft.Extensions.DependencyInjection;

namespace Pokemon.Battle.Events;

/// <summary>
/// Provides extension methods for configuring event-related services
/// within the Pokémon battle system, specifically targeting abilities
/// and their associated event configurations.
/// </summary>
public static class EventExtensions
{
    /// <summary>
    /// Configures ability-related services and their associated event handlers
    /// within the Pokémon battle system. This extension method allows the
    /// customization of ability events, enabling specific behaviors during
    /// battle calculations.
    /// </summary>
    /// <param name="services">The IServiceCollection to add the ability event configurations to.</param>
    /// <param name="configure">
    /// An Action delegate that defines the configuration of the AbilityEvents,
    /// allowing the registration of ability-specific event handlers.
    /// </param>
    /// <returns>
    /// The IServiceCollection instance with the ability event configurations applied,
    /// allowing for further chaining of service configurations.
    /// </returns>
    public static IServiceCollection ConfigureAbilities(
        this IServiceCollection services,
        Action<AbilityEvents> configure
    )
    {
        return services.Configure(configure);
    }
}
