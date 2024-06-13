// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Primatives/SelectableOption.h"
#include "BattleMenuOption.generated.h"

class UBattleMenuHandler;
class UImage;
class UDisplayText;
/**
 * The actual option widget for the move selection.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleMenuOption : public USelectableOption {
    GENERATED_BODY()

public:
    /**
     * Init the properties of this option with the given handler
     * @param Handler The handler to use for initialization
     */
    void InitFromHandler(const UBattleMenuHandler* Handler);

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

};
