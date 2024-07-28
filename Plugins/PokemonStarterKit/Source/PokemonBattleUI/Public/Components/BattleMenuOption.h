// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/RPGButtonBase.h"

#include "BattleMenuOption.generated.h"

class UBattleMenuHandler;
class UImage;
class UDisplayText;
/**
 * The actual option widget for the move selection.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMenuOption : public URPGButtonBase {
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;
    void NativeOnCurrentTextStyleChanged() override;

  private:
    /**
     * The widget that displays the option text
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> OptionText;

    /**
     * The widget that displays the background widget
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundImage;

    /**
     * The text used for the option
     */
    UPROPERTY(EditAnywhere, Category = Option)
    FText Text;

    /**
     * The image to use for the option's background
     */
    UPROPERTY(EditAnywhere, Category = Option)
    FSlateBrush Image;
};
