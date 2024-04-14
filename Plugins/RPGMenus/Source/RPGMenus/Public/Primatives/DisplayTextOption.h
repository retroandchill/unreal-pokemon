// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SelectableOption.h"

#include "DisplayTextOption.generated.h"

class UDisplayText;
/**
 * Selectable option that contains a display text that can be updated
 */
UCLASS(Abstract)
class RPGMENUS_API UDisplayTextOption : public USelectableOption {
    GENERATED_BODY()

  public:
    /**
     * Set the display text for this widget
     * @param NewText The new display text for the widget
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void SetText(const FText &NewText);

    /**
     * Get the size of the current text contained within this widget
     * @return The size of the current text
     */
    FVector2D GetTextSize() const;

    /**
     * Get the padding around the display text for the player
     * @return The padding around the text
     */
    UFUNCTION(BlueprintCallable, Category = "Text|Size")
    FMargin GetDisplayTextPadding() const;

  private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> DisplayText;
};
