using Pokemon.Data.HardCoded;
using UnrealSharp.Engine.Core.Modules;

namespace Pokemon.Data;

public class FPokemonDataModule : IModuleInterface
{
    public void StartupModule()
    {
        UGameDataManager.Startup();

        UGameDataManager.Instance.AddGrowthRates();
    }

    public void ShutdownModule() { }
}
