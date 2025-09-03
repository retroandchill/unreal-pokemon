// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "GiveItemToPokemon.generated.h"

class IPokemon;

/**
 * Delegate for the result of the item in question
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemResult);

/**
 * Give an item to a Pokémon to hold, and take a held item from a Pokémon.
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UGiveItemToPokemon : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Give an item to a Pokémon to hold, and take a held item from a Pokémon
     * @param WorldContextObject The object used to obtain the state of the world
     * @param Item The new item to give
     * @param Pokemon The Pokémon receiving the item
     * @param PokemonIndex The index of the Pokémon in question
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Selection")
    static UGiveItemToPokemon *GiveItemToPokemon(const UObject *WorldContextObject, FItemHandle Item,
                                                 const TScriptInterface<IPokemon> &Pokemon, int32 PokemonIndex = 0);

  protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine = {}) override;

  private:
    /**
     * Called when the item is given to the Pokémon
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemGiven;

    /**
     * Called when the player backs out (or the item cannot be given)
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemRejected;

    /**
     * The new item to give
     */
    UPROPERTY()
    FName Item;

    /**
     * The Pokémon receiving the item
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The index of the Pokémon in question
     */
    UPROPERTY()
    int32 PokemonIndex;
};
