// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "BattleMenuHandler.generated.h"

class UPokemonBattleScreen;
/**
 * Represents a handler in the battle menu.
 */
UCLASS(Abstract, EditInlineNew)
class POKEMONBATTLEUI_API UBattleMenuHandler : public UObject {
    GENERATED_BODY()

  public:
    /**
     * The text for the option in question.
     * @return The text used for the option
     */
    const FText &GetOptionText() const;

    /**
     * The visuals for the background of the option
     * @return The image to draw behind the text
     */
    const FSlateBrush &GetBackgroundImage() const;

    /**
     * Called when this option is selected.
     * @param BattleScreen The screen that this command takes place in the context of
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Battle|Selection")
    void OnSelected(UPokemonBattleScreen *BattleScreen);

  private:
    /**
     * The text used for the option
     */
    UPROPERTY(EditAnywhere, Category = Option)
    FText OptionText;

    /**
     * The image to use for the option's background
     */
    UPROPERTY(EditAnywhere, Category = Option)
    FSlateBrush BackgroundImage;
};
