// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GiveItemToPokemon.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "TakeItemFromPokemon.generated.h"

class IPokemon;

/**
 * Take a Pokémon's hold item and return it to the bag
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UTakeItemFromPokemon : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Give an item to a Pokémon to hold, and take a held item from a Pokémon
     * @param Pokemon The Pokémon receiving the item
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", CallableWithoutWorldContext = true), Category = "Selection")
    static UTakeItemFromPokemon *TakeItemFromPokemon(const TScriptInterface<IPokemon> &Pokemon);

    protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called when the item is taken from the Pokémon
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemTaken;

    /**
     * Called when the player backs out (or the item cannot be taken)
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemNotTaken;

    /**
     * The Pokémon receiving the item
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;
};
