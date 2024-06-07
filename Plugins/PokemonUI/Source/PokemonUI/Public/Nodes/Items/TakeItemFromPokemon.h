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
class POKEMONUI_API UTakeItemFromPokemon : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

  public:
    /**
     * Give an item to a Pokémon to hold, and take a held item from a Pokémon
     * @param WorldContextObject The object used to obtain the state of the world
     * @param Pokemon The Pokémon receiving the item
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Selection")
    static UTakeItemFromPokemon *TakeItemFromPokemon(const UObject *WorldContextObject,
                                                     const TScriptInterface<IPokemon> &Pokemon);

    void Activate() override;

  private:
    /**
     * Function called to execute the item taken pin
     */
    UFUNCTION()
    void ExecuteItemTaken();

    /**
     * Function called to execute the item not taken pin
     */
    UFUNCTION()
    void ExecuteItemNotTaken();

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
     * The object used to obtain the state of the world to open the menu with
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContextObject;

    /**
     * The Pokémon receiving the item
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;

    /**
     * The utility object used to call special macros
     */
    UPROPERTY()
    TObjectPtr<UObject> ItemUtilities;
};
