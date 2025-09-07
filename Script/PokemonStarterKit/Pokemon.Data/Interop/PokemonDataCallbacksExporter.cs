using UnrealSharp.Binds;

namespace Pokemon.Data.Interop;

[NativeCallbacks]
public static unsafe partial class PokemonDataCallbacksExporter
{
    private static readonly delegate* unmanaged<ref PokemonDataManagedActions, void> SetCallbacks;
}
