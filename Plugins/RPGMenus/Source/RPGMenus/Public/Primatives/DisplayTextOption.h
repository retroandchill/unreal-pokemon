// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "SelectableOption.h"

#include "DisplayTextOption.generated.h"

class UCommonTextBlock;
class UDisplayText;
/**
 * Selectable option that contains a display text that can be updated
 */
UCLASS(Abstract)
class RPGMENUS_API UDisplayTextOption : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    /**
     * Set the display text for this widget
     * @param NewText The new display text for the widget
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void SetText(const FText &NewText);

protected:
    void NativeOnCurrentTextStyleChanged() override;

  private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> DisplayText;
};
