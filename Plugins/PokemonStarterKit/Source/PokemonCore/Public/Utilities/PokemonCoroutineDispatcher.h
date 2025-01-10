// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Lookup/InjectableDependency.h"
#include "UE5Coro.h"
#include "Moves/MoveData.h"
#include "UObject/Interface.h"

#include "PokemonCoroutineDispatcher.generated.h"

struct FLevelUpStatChanges;
class IPokemon;

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

    virtual UE5Coro::TCoroutine<bool> GiveItemToPokemon(const FItemHandle &Item,
                                                        const TScriptInterface<IPokemon> Pokemon,
                                                        int PokemonIndex, FForceLatentCoroutine Coro = {}) const = 0;

    virtual UE5Coro::TCoroutine<bool> TakeItemFromPokemon(const TScriptInterface<IPokemon> &Pokemon, FForceLatentCoroutine Coro = {}) const = 0;

    virtual UE5Coro::TCoroutine<bool> LearnMove(const TScriptInterface<IPokemon> &Pokemon, FMoveHandle Move, FForceLatentCoroutine Coro = {}) const = 0;

    virtual UE5Coro::TCoroutine<> ProcessLevelUp(const TScriptInterface<IPokemon> &Pokemon, const FLevelUpStatChanges& StatChanges, FForceLatentCoroutine Coro = {}) const = 0;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, IPokemonCoroutineDispatcher)