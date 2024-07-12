// "Unreal Pokémon" created by Retro & Chill.


#include "Actions/PokemonActionBoundButton.h"
#include "CommonInputSubsystem.h"

void UPokemonActionBoundButton::NativeConstruct() {
    Super::NativeConstruct();

    if (auto InputSubsystem = GetInputSubsystem(); InputSubsystem != nullptr) {
        InputSubsystem->OnInputMethodChangedNative.AddUObject(this, &ThisClass::HandleInputMethodChanged);
        HandleInputMethodChanged(InputSubsystem->GetCurrentInputType());
    }
}

void UPokemonActionBoundButton::NativeConstruct() {
    Super::NativeConstruct();
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