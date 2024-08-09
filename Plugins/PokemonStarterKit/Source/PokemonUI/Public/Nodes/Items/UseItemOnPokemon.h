// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GiveItemToPokemon.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UseItemOnPokemon.generated.h"

class UBagScreen;
class IPokemon;
/**
 * 
 */
UCLASS(meta = (HideThen))
class POKEMONUI_API UUseItemOnPokemon : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Use the given item on the specified Pokémon.
     * @param Screen The screen calling the teim
     * @param Item The item being used
     * @param Pokemon The Pokémon receiving the item
     * @return The created node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Items,
        meta = (DefaultToSelf = Screen))
    static UUseItemOnPokemon *UseItemOnPokemon(UBagScreen *Screen, FName Item, int32 Quantity, const TScriptInterface<IPokemon> &Pokemon);

    void Activate() override;

private:
    /**
     * Called when the item is given to the Pokémon
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemUsed;

    /**
     * Called when the player backs out (or the item cannot be given)
     */
    UPROPERTY(BlueprintAssignable)
    FItemResult ItemNotUsed;

    /**
     * The object used to obtain the state of the world to open the menu with
     */
    UPROPERTY()
    TObjectPtr<UBagScreen> BagScreen;

    /**
     * The new item to give
     */
    UPROPERTY()
    FName Item;

    /**
     * The new item to give
     */
    UPROPERTY()
    int32 Quantity;

    /**
     * The Pokémon receiving the item
     */
    UPROPERTY()
    TScriptInterface<IPokemon> Pokemon;
};
