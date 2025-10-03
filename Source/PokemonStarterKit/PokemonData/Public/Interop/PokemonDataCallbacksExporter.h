// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UnrealSharpBinds/Public/CSBindsManager.h"
#include "UObject/Object.h"

#include "PokemonDataCallbacksExporter.generated.h"

struct FPokemonDataManagedActions;

/**
 * @class UPokemonDataCallbacksExporter
 * @brief A UObject class responsible for exporting callback functions related to Pokemon data management.
 *
 * This class provides functionality to set callback actions for managing Pokemon data using UnrealSharp.
 * It interacts with FPokemonDataManagedCallbacks to register user-defined callback actions.
 *
 * @note The class is marked with meta = (SkipGlueGeneration) to skip specific code generation in Unreal.
 */
UCLASS(meta = (SkipGlueGeneration))
class POKEMONDATA_API UPokemonDataCallbacksExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Sets the callback functions for Pokemon data management operations.
     *
     * This static method allows the user to provide a set of callback functions
     * that will be used to manage operations within the Pokemon data system.
     * It registers the callbacks with the FPokemonDataManagedCallbacks instance.
     *
     * @param InCallbacks A reference to an FPokemonDataManagedActions structure containing
     * the callback functions to be set for managing Pokemon data.
     */
    UNREALSHARP_FUNCTION()
    static void SetCallbacks(const FPokemonDataManagedActions &InCallbacks);
};
