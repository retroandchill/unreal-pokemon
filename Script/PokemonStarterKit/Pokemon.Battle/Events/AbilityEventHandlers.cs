using Microsoft.Extensions.Options;
using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;
using Retro.ReadOnlyParams.Annotations;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Events;

/// <summary>
/// Represents a collection of ability-related events and their configurations
/// within the battle system. This class is used to manage and register event
/// handlers associated with Pokémon abilities, such as modifying gameplay calculations
/// or applying specific effects during the battle process.
/// </summary>
[Service]
public class AbilityEvents
{
    /// <summary>
    /// Represents a delegate for calculating the speed of a Pokémon during battle.
    /// This delegate is used to define custom speed calculation logic based on the
    /// battler's state and an optional multiplier. It is commonly utilized
    /// by abilities to override or modify default speed values.
    /// </summary>
    public Dictionary<FAbilityHandle, SpeedCalc> SpeedCalc { get; } = new();
}

/// <summary>
/// Provides mechanisms to handle and trigger custom ability-related event logic
/// during gameplay. This class is specifically designed to manage callbacks or
/// computations tied to specific Pokémon abilities, including modifying stats
/// like speed through specified calculations based on certain conditions.
/// </summary>
/// <param name="events">The ability events configuration.</param>
[Service]
public class AbilityEventHandlers(IOptions<AbilityEvents> events)
{
    private readonly AbilityEvents _abilityEvents = events.Value;

    /// <summary>
    /// Triggers the speed calculation for a Pokémon's ability, modifying the speed stat
    /// based on the ability's specific logic and conditions. If no calculation is found
    /// for the provided ability, the method returns the provided multiplier unchanged.
    /// </summary>
    /// <param name="ability">The handle representing the Pokémon's ability.</param>
    /// <param name="battler">The battler instance whose speed is being recalculated.</param>
    /// <param name="multiplier">The initial speed multiplier to apply during the computation. Default is 1.0.</param>
    /// <returns>The resulting speed multiplier after applying the ability-specific calculation, or the unchanged initial multiplier if no calculation is found.</returns>
    public float TriggerSpeedCalc(FAbilityHandle ability, UBattler battler, float multiplier = 1.0f)
    {
        return _abilityEvents.SpeedCalc.TryGetValue(ability, out var calc) ? calc(battler, multiplier) : multiplier;
    }
}
