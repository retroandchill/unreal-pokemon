// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "BagScreen.generated.h"

class UPocketWindow;
class UItemInfoWindow;
class UItemSelectionWindow;

/**
 * The screen that contains the basic outline of the player's bag
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UBagScreen : public UScreen {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

private:
    /**
     * The window that displays the pockets
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPocketWindow> PocketWindow;

    /**
     * The window that displays the item list
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UItemSelectionWindow> ItemSelectionWindow;

    /**
     * The window that displays the item information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UItemInfoWindow> ItemInfoWindow;
};
