using Pokemon.Core.Entities;
using Pokemon.Core.Services.Exp;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;

namespace Pokemon.Core;

/// <summary>
/// Provides static properties and methods related to Pokémon game mechanics.
/// </summary>
public static class PokemonStatics
{
    /// <summary>
    /// Represents the maximum number of Pokémon that a trainer can hold in their party at one time.
    /// </summary>
    /// <remarks>
    /// This property retrieves the default configuration value defined in the Pokémon Core settings.
    /// </remarks>
    public static int MaxPartySize => UObject.GetDefault<UPokemonCoreSettings>().MaxPartySize;

    /// <summary>
    /// Specifies the maximum number of moves a Pokémon can know at any given time.
    /// </summary>
    /// <remarks>
    /// This property reflects the default value configured in Pokémon Core settings.
    /// </remarks>
    public static int MaxMoves => UObject.GetDefault<UPokemonCoreSettings>().MaxMoves;

    /// <summary>
    /// Represents the chance of a Pokémon being shiny as defined in the game's configuration settings.
    /// </summary>
    /// <remarks>
    /// This property retrieves the default shiny chance value specified in the Pokémon Core settings,
    /// which determines how likely a Pokémon is to be generated as shiny.
    /// </remarks>
    public static int ShininessChance => UObject.GetDefault<UPokemonCoreSettings>().ShinyPokemonChance;

    internal static UObject WorldContextObject
    {
        get
        {
            var worldContextObject = FCSManagerExporter.CallGetCurrentWorldContext();
            var worldContextHandle = FCSManagerExporter.CallFindManagedObject(worldContextObject);
            return GCHandleUtilities.GetObjectFromHandlePtr<UObject>(worldContextHandle)!;
        }
    }

    /// <summary>
    /// Represents the player's trainer instance in the current game session.
    /// </summary>
    /// <remarks>
    /// This property provides access to the current player's trainer entity, which includes components and functionality for managing party Pokémon, trainer info, and other player-related behavior.
    /// </remarks>
    public static UTrainer Player => WorldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().Player;

    /// <summary>
    /// Retrieves the experience growth formula associated with the specified growth rate.
    /// </summary>
    /// <param name="growthRate">
    /// The <see cref="FGrowthRateHandle"/> representing the growth rate for which the experience growth formula is requested.
    /// </param>
    /// <returns>
    /// An <see cref="IExpGrowthFormula"/> instance that dictates the experience growth behavior for the provided growth rate.
    /// </returns>
    public static IExpGrowthFormula GetExpGrowthFormula(FGrowthRateHandle growthRate)
    {
        return WorldContextObject.GetGameInstanceSubsystem<UPokemonSubsystem>().GetExpGrowthFormula(growthRate);
    }
}
