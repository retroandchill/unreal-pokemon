// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "RetroLib/Ranges/CircularIterator.h"

#include "PocketTabWidget.generated.h"

class UItemSelectionWindow;
class UCommonActionWidget;
class UCommonButtonStyle;
class UPocketButton;

/**
 * Widget the contains the information about the selected pockets in the bag scene.
 */
UCLASS(Abstract)
class POKEMONUI_API UPocketTabWidget : public UCommonUserWidget {
    GENERATED_BODY()

  public:
    UPocketTabWidget();

  protected:
    void NativePreConstruct() override;
    void NativeConstruct() override;

    UFUNCTION(BlueprintImplementableEvent, Category = Buttons)
    void SlotButton(UPocketButton *Button);

  public:
    void SetItemSelectionWindow(UItemSelectionWindow *Window);

  private:
    UPocketButton *CreatePocketButton(FName Pocket);

    void PocketLeft();
    void PocketRight();
    void OnPocketClicked(int32 Index);

    UPROPERTY(EditAnywhere, Category = Buttons)
    TSubclassOf<UPocketButton> ButtonClass;

    UPROPERTY(EditAnywhere, EditFixedSize, Category = Buttons)
    TMap<FName, TSubclassOf<UCommonButtonStyle>> PocketButtonStyles;

    UPROPERTY()
    TArray<TObjectPtr<UPocketButton>> PocketButtons;

    UPROPERTY()
    TObjectPtr<UCommonButtonGroupBase> PocketButtonGroup;

    UPROPERTY()
    TObjectPtr<UItemSelectionWindow> ItemSelectionWindow;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> PocketLeftAction;

    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> PocketRightAction;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonActionWidget> PocketLeftActionWidget;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonActionWidget> PocketRightActionWidget;

    Retro::TCircularIterator<const FName> CurrentPocket;

    FUIActionBindingHandle PocketLeftBinding;
    FUIActionBindingHandle PocketRightBinding;
};
