// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Kismet/BlueprintAsyncActionBase.h"

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
class POKEMONUI_API UGiveItemToPokemon : public UBlueprintAsyncActionBase {
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

    void Activate() override;

  private:
    /**
     * Function called to execute the item given pin
     */
    UFUNCTION()
    void ExecuteItemGiven();

    /**
     * Function called to execute the item rejected pin
     */
    UFUNCTION()
    void ExecuteItemRejected();

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
     * The object used to obtain the state of the world to open the menu with
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;

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
