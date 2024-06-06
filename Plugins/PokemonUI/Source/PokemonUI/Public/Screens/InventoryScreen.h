// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Handlers/PartyMenu/RemovableScreen.h"
#include "UObject/Interface.h"
#include "InventoryScreen.generated.h"

class IPokemon;
struct FItem;
/**
 * Delegate for when an item is selected
 */
DECLARE_DELEGATE_ThreeParams(FOnItemSelected, const TScriptInterface<class IInventoryScreen>&, const FItem&, int32);

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UInventoryScreen : public UInterface {
    GENERATED_BODY()
};

/**
 * Abstract declaration of the inventory screen used by the game
 */
class POKEMONUI_API IInventoryScreen : public IRemovableScreen {
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

    UFUNCTION(BlueprintCallable, Category = Navigation)
    virtual void RemoveFromStack() override = 0;

    UFUNCTION(BlueprintCallable, Category = Display)
    virtual void RefreshScene() = 0;

};