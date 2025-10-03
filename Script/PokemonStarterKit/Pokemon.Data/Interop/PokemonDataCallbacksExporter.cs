using UnrealSharp.Binds;

namespace Pokemon.Data.Interop;

/// <summary>
/// Provides an export interface for native callbacks related to Pokémon data.
/// </summary>
/// <remarks>
/// This static class acts as a bridge between managed and unmanaged code, offering
/// functionalities to bind and set callback actions related to Pokémon data management.
/// It enables the integration of managed actions with native systems for various data processing and operations.
/// The class is specifically marked with <c>[NativeCallbacks]</c> attribute and is part of the
/// <c>Pokemon.Data.Interop</c> namespace.
/// </remarks>
[NativeCallbacks]
public static unsafe partial class PokemonDataCallbacksExporter
{
    private static readonly delegate* unmanaged<ref PokemonDataManagedActions, void> SetCallbacks;
}
