// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonActionBoundButton.h"
#include "CommonActionWidget.h"
#include "CommonInputSubsystem.h"
#include "CommonTextBlock.h"
#include "Input/UIActionBinding.h"

void UPokemonActionBoundButton::NativeConstruct() {
    Super::NativeConstruct();

    if (auto InputSubsystem = GetInputSubsystem(); InputSubsystem != nullptr) {
        InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
        HandleInputMethodChanged(InputSubsystem->GetCurrentInputType());
    }
}

void UPokemonActionBoundButton::SetRepresentedAction(FUIActionBindingHandle InBindingHandle) {
    if (auto OldBinding = FUIActionBinding::FindBinding(BindingHandle); OldBinding != nullptr) {
        OldBinding->OnHoldActionProgressed.RemoveAll(this);
    }

    BindingHandle = InBindingHandle;
    UpdateInputActionWidget();

    if (auto NewBinding = FUIActionBinding::FindBinding(InBindingHandle); NewBinding != nullptr) {
        NewBinding->OnHoldActionProgressed.AddUObject(this, &UPokemonActionBoundButton::NativeOnActionProgress);
    }
}

void UPokemonActionBoundButton::NativeOnClicked() {
    Super::NativeOnClicked();
    if (auto ActionBinding = FUIActionBinding::FindBinding(BindingHandle); ActionBinding != nullptr) {
        ActionBinding->OnExecuteAction.ExecuteIfBound();
    }
}

void UPokemonActionBoundButton::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();

    if (Text_ActionName != nullptr) {
        Text_ActionName->SetStyle(GetCurrentTextStyleClass());
    }
}

void UPokemonActionBoundButton::UpdateInputActionWidget() {
    if (InputActionWidget != nullptr) { // optional bound widget
        InputActionWidget->SetInputActionBinding(BindingHandle);

        FText ActionDisplayName;
        if (auto Binding = FUIActionBinding::FindBinding(BindingHandle); Binding != nullptr) {
            ActionDisplayName = Binding->ActionDisplayName;
        }

        if (ActionDisplayName.IsEmpty()) {
            ActionDisplayName = BindingHandle.GetDisplayName();
        }
        if (BindingHandle.IsValid()) {
            const auto BoundWidget = BindingHandle.GetBoundWidget();
            if (auto BindingOwner = BoundWidget ? BoundWidget->GetOwningLocalPlayer() : nullptr;
                ensure(BindingOwner != nullptr) && BindingOwner != GetOwningLocalPlayer()) {
                int32 BoundPlayerIndex =
                    FSlateApplication::Get().GetUserIndexForController(BindingOwner->GetControllerId());
                ActionDisplayName = FText::FormatNamed(
                    NSLOCTEXT("PokemonActionBoundButton", "OtherPlayerActionFormat", "[P{PlayerNum}] {ActionName}"),
                    TEXT("PlayerNum"), FText::AsNumber(BoundPlayerIndex + 1), TEXT("ActionName"), ActionDisplayName);
            }
        }

        if (Text_ActionName != nullptr) {
            Text_ActionName->SetText(ActionDisplayName);
        }

        OnUpdateInputAction();
    }
}

UCommonTextBlock *UPokemonActionBoundButton::GetActionNameText() const {
    return Text_ActionName;
}

void UPokemonActionBoundButton::HandleInputMethodChanged(ECommonInputType NewInputMethod) {
    using enum ECommonInputType;

    TSubclassOf<UCommonButtonStyle> NewStyle;
    switch (NewInputMethod) {
    case Gamepad:
        NewStyle = GamepadStyle;
        break;
    case Touch:
        NewStyle = TouchStyle;
        break;
    default:
        NewStyle = KeyboardStyle;
        break;
    }

    if (NewStyle != nullptr) {
        SetStyle(NewStyle);
    }
}