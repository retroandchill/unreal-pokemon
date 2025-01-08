// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Lookup/InjectableDependency.h"
#include "UE5Coro.h"
#include "Bag/Item.h"
#include "UObject/Interface.h"
#include "PokemonCoroutineDispatcher.generated.h"

class IPokemon;
struct FItem;
// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class POKEMONCORE_API UPokemonCoroutineDispatcher : public UInterface {
    GENERATED_BODY()
};

/**
 * 
 */
class POKEMONCORE_API IPokemonCoroutineDispatcher {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    static const IPokemonCoroutineDispatcher &Get(const UObject *WorldContext);
    
    virtual UE5Coro::TCoroutine<bool> GiveItemToPokemon(const UObject* WorldContext, const FItemHandle& Item, const TScriptInterface<IPokemon> Pokemon, int PokemonIndex) const = 0;
    
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, IPokemonCoroutineDispatcher)