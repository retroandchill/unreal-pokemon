using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Core.Services.Stats;

/// <summary>
/// Provides methods for performing stat calculations within the context of Pokémon data processing.
/// </summary>
public interface IStatCalculationService
{
    /// <summary>
    /// Calculates the value of a Pokémon's stat based on its base value,
    /// level, IV (Individual Value), EV (Effort Value), and nature modifier.
    /// </summary>
    /// <param name="stat">
    /// The specific stat being calculated, represented by an FMainStatHandle.
    /// </param>
    /// <param name="baseValue">
    /// The base value of the stat as defined by the Pokémon's species.
    /// </param>
    /// <param name="level">
    /// The level of the Pokémon.
    /// </param>
    /// <param name="iv">
    /// The Individual Value (IV) of the stat, contributing to its unique growth.
    /// </param>
    /// <param name="ev">
    /// The Effort Value (EV) of the stat, influencing its growth through training.
    /// </param>
    /// <param name="natureModifer">
    /// A percentage modifier applied to the stat based on the Pokémon's nature (e.g., a 10% boost or decrease).
    /// </param>
    /// <returns>
    /// The calculated stat value after applying all modifiers and contributions.
    /// </returns>
    int CalculateStat(FMainStatHandle stat, int baseValue, int level, int iv, int ev, int natureModifer);
}
