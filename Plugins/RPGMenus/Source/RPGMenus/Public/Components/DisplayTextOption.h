// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "RPGButtonBase.h"

#include "DisplayTextOption.generated.h"

class UDisplayText;
/**
 * Selectable option that contains a display text that can be updated
 */
UCLASS(Abstract)
class RPGMENUS_API UDisplayTextOption : public URPGButtonBase {
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;

  public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const FText &GetText() const;

    /**
     * Set the display text for this widget
     * @param NewText The new display text for the widget
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content)
    void SetText(const FText &NewText);

  protected:
    void NativeOnCurrentTextStyleChanged() override;

  private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetText, BlueprintSetter = SetText, Category = Content)
    FText Text = FText::FromStringView(TEXT("Text Block"));

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> DisplayText;
};
