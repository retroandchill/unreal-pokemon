// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

#include "PokemonCoroutineDispatcherImpl.generated.h"

/**
 *
 */
UCLASS()
class POKEMONUI_API UPokemonCoroutineDispatcherImpl : public UObject, public IPokemonCoroutineDispatcher {
    GENERATED_BODY()

  public:
    UE5Coro::TCoroutine<bool> GiveItemToPokemon(const UObject *WorldContext, const FItemHandle &Item,
                                                const TScriptInterface<IPokemon> Pokemon,
                                                int PokemonIndex) const override;
    
    UE5Coro::TCoroutine<bool> TakeItemFromPokemon(const UObject *WorldContext, const TScriptInterface<IPokemon> &Pokemon) const override;
};
