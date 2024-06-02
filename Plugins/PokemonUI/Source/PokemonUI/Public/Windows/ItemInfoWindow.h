// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ItemInfoWindow.generated.h"

class UDisplayText;
class UItemSelectionWindow;
class UImage;

/**
 * Window for displaying the information about an item.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UItemInfoWindow : public UUserWidget {
    GENERATED_BODY()

  public:
    
    
  private:
    /**
     * The graphic for the icon of the item.
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemIcon;

    /**
     * The text used to show the item's description
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ItemDescription;
};
