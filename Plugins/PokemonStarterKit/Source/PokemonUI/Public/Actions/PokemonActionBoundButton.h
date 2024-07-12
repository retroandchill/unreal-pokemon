// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionButton.h"
#include "PokemonActionBoundButton.generated.h"

/**
 * Specialized native action button implementation used to handling the changing of styles when the input method does
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class POKEMONUI_API UPokemonActionBoundButton : public UCommonBoundActionButton {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;

private:
    /**
     * Handle the appearance when the layout changes
     * @param NewInputMethod The new method of input to use
     */
    void HandleInputMethodChanged(ECommonInputType NewInputMethod);

    /**
     * The style used for keyboard and mouse input
     */
    UPROPERTY(EditAnywhere, Category = "Styles")
    TSubclassOf<UCommonButtonStyle> KeyboardStyle;

    /**
     * The style used during gamepad input
     */
    UPROPERTY(EditAnywhere, Category = "Styles")
    TSubclassOf<UCommonButtonStyle> GamepadStyle;

    /**
     * The style used for touch input
     */
    UPROPERTY(EditAnywhere, Category = "Styles")
    TSubclassOf<UCommonButtonStyle> TouchStyle;
};
