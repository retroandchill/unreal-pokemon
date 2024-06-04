// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/ScreenInterface.h"
#include "UObject/Interface.h"
#include "InventoryScreen.generated.h"

class IPokemon;
struct FItem;
/**
 * Delegate for when an item is selected
 */
DECLARE_DELEGATE_ThreeParams(FOnItemSelected, const TScriptInterface<class IInventoryScreen>&, const FItem&, int32);

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UInventoryScreen : public UInterface {
    GENERATED_BODY()
};

/**
 * Abstract declaration of the inventory screen used by the game
 */
class POKEMONUI_API IInventoryScreen 
#if CPP
    : public virtual IScreenInterface {
#else
    : public IScreenInterface {
#endif
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Callback override for when an item is selected. When this is bound it will override the default behavior
     * for when an item is selected.
     * @return The callback for when an item is selected by the player
     */
    virtual FOnItemSelected& GetOnItemSelected() = 0;

    /**
     * Toggle whether the player can select an item or not in the menu
     * @param bCanSelect Can the player select an item
     */
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    virtual void ToggleItemSelection(bool bCanSelect) = 0;

        virtual void UseItemOnPokemon(const FItem& Item, int32 Quantity, const TScriptInterface<IPokemon>& Pokemon) {
            
        }

};