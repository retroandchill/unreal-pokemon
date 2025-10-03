using System.Diagnostics.CodeAnalysis;
using JetBrains.Annotations;
using Pokemon.Data.Interop;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Data;

/// <summary>
/// Represents the data module for managing Pokémon-related game data within the application.
/// </summary>
/// <remarks>
/// This class serves as the central module responsible for initializing, managing, and disposing of Pokémon game-related data.
/// It interacts with various data components, including growth rates, habitats, evolution chains, and other gameplay systems.
/// </remarks>
[UsedImplicitly]
public class FPokemonDataModule : IModuleInterface
{
    private static readonly Lock Lock = new();

    /// <summary>
    /// Gets or sets the singleton instance of the <see cref="FPokemonDataModule" /> class.
    /// </summary>
    /// <remarks>
    /// This property provides access to the global instance of the <see cref="FPokemonDataModule" /> class.
    /// It is initialized during the startup process and disposed during shutdown.
    /// The instance is required for managing Pokémon game-related data and interacting with the relevant game systems.
    /// Attempting to access this property before it is initialized will result in an <see cref="InvalidOperationException" />.
    /// </remarks>
    /// <exception cref="InvalidOperationException">
    /// Thrown when attempting to access the instance before it has been initialized.
    /// </exception>
    [field: AllowNull, MaybeNull]
    public static FPokemonDataModule Instance
    {
        get => field ?? throw new InvalidOperationException("PokemonDataModule is not initialized");
        private set
        {
            lock (Lock)
            {
                field = value;
            }
        }
    }

    /// <summary>
    /// Event triggered upon the creation and initialization of the <see cref="UGameDataManager" /> instance.
    /// </summary>
    /// <remarks>
    /// This event is invoked after the <see cref="UGameDataManager" /> has been successfully created and all associated game data,
    /// including growth rates, habitats, evolutions, and other Pokémon gameplay elements, have been loaded.
    /// Subscribers can use this event to perform additional setup or interact with the initialized data manager.
    /// </remarks>
    [PublicAPI]
    public event Action<UGameDataManager>? OnGameDataManagerCreated;

    /// <inheritdoc />
    public void StartupModule()
    {
        var callbacks = PokemonDataManagedActions.Create();
        PokemonDataCallbacksExporter.CallSetCallbacks(ref callbacks);
        Instance = this;
    }

    /// <inheritdoc />
    public void ShutdownModule()
    {
        Instance = null!;
    }

    internal void CreateGameDataManager()
    {
        UGameDataManager.Startup();

        var manager = UGameDataManager.Instance;
        manager
            .AddGrowthRates()
            .AddGenderRatios()
            .AddEggGroups()
            .AddBodyShapes()
            .AddBodyColors()
            .AddHabitats()
            .AddEvolutions()
            .AddStats()
            .AddNatures()
            .AddStatuses()
            .AddFieldWeathers()
            .AddEncounterTypes()
            .AddEnvironments()
            .AddBattleWeathers()
            .AddBattleTerrains()
            .AddTargets();
        OnGameDataManagerCreated?.Invoke(manager);
    }
}
