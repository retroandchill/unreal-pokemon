// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ItemInfoWindow.generated.h"

class UDisplayText;
class UItemSelectionWindow;
class UImage;
struct FItem;

/**
 * Window for displaying the information about an item.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UItemInfoWindow : public UUserWidget
{
    GENERATED_BODY()

  public:
    /**
     * Refresh the selected item and quantity
     * @param Item The item in question
     * @param Quantity The number of items held
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Display)
    void Refresh(const FItem &Item, int32 Quantity);

    /**
     * Clear out any item information.
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = Display)
    void ClearItem();
};
