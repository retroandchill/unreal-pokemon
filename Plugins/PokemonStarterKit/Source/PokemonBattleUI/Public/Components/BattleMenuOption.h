// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "BattleMenuOption.generated.h"

class UCommonTextBlock;
class UBattleMenuHandler;
class UImage;
/**
 * The actual option widget for the move selection.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMenuOption : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    /**
     * Init the properties of this option with the given handler
     * @param Handler The handler to use for initialization
     */
    void InitFromHandler(const UBattleMenuHandler *Handler);

protected:
    void NativeOnCurrentTextStyleChanged() override;

  private:
    /**
     * The widget that displays the option text
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> OptionText;

    /**
     * The widget that displays the background widget
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundImage;
};
