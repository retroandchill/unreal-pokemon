// "Unreal Pokémon" created by Retro & Chill.

#include "Components/DisplayTextOption.h"
#include "Components/DisplayText.h"

void UDisplayTextOption::SetText(const FText &NewText) {
    DisplayText->SetText(NewText);
}

void UDisplayTextOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    if (DisplayText != nullptr) {
        DisplayText->SetTextStyle(GetCurrentTextStyleClass());
    }
}