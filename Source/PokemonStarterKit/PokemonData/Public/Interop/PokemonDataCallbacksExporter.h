// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UnrealSharpBinds/Public/CSBindsManager.h"
#include "UObject/Object.h"

#include "PokemonDataCallbacksExporter.generated.h"

struct FPokemonDataManagedActions;
/**
 *
 */
UCLASS(meta = (SkipGlueGeneration))
class POKEMONDATA_API UPokemonDataCallbacksExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static void SetCallbacks(const FPokemonDataManagedActions &InCallbacks);
};
