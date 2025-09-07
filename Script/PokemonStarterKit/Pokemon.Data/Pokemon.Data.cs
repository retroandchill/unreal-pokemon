using System.Diagnostics.CodeAnalysis;
using JetBrains.Annotations;
using Pokemon.Data.Interop;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Data;

[UsedImplicitly]
public class FPokemonDataModule : IModuleInterface
{
    private static readonly Lock Lock = new();

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

    [PublicAPI]
    public event Action<UGameDataManager>? OnGameDataManagerCreated;

    public void StartupModule()
    {
        var callbacks = PokemonDataManagedActions.Create();
        PokemonDataCallbacksExporter.CallSetCallbacks(ref callbacks);
        Instance = this;
    }

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
