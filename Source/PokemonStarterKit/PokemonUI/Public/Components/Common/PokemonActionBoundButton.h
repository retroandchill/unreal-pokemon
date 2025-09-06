// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/CommonBoundActionButton.h"

#include "PokemonActionBoundButton.generated.h"

class UTextBlock;

/**
 * Specialized native action button implementation used to handling the changing of styles when the input method does
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class POKEMONUI_API UPokemonActionBoundButton : public UCommonButtonBase, public ICommonBoundActionButtonInterface
{
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
    virtual void SetRepresentedAction(FUIActionBindingHandle InBindingHandle) override;

  protected:
    void NativeOnClicked() override;
    void NativeOnCurrentTextStyleChanged() override;

    void UpdateInputActionWidget() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Common Bound Action")
    void OnUpdateInputAction();

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Text Block")
    UCommonTextBlock *GetActionNameText() const;

  private:
    /**
     * Handle the appearance when the layout changes
     * @param NewInputMethod The new method of input to use
     */
    void HandleInputMethodChanged(ECommonInputType NewInputMethod);

    UPROPERTY(BlueprintGetter = GetActionNameText, Category = "Text Block", meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> Text_ActionName;

    FUIActionBindingHandle BindingHandle;

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
