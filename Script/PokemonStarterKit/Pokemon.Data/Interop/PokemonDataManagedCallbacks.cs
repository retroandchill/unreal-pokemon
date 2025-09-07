using System.Runtime.InteropServices;

namespace Pokemon.Data.Interop;

public readonly unsafe struct PokemonDataManagedActions
{
    public required delegate* unmanaged<void> StartDataManager { get; init; }

    public static PokemonDataManagedActions Create()
    {
        return new PokemonDataManagedActions
        {
            StartDataManager = &PokemonDataManagedCallbacks.StartDataManager,
        };
    }
}

public static class PokemonDataManagedCallbacks
{
    [UnmanagedCallersOnly]
    public static void StartDataManager()
    {
        FPokemonDataModule.Instance.CreateGameDataManager();
    }
}
