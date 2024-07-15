// "Unreal Pokémon" created by Retro & Chill.

#include "Components/DisplayTextOption.h"
#include "Primatives/DisplayText.h"

void UDisplayTextOption::SetText(const FText &NewText) {
    DisplayText->SetText(NewText);
}

void UDisplayTextOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    //DisplayText->SetTextStyle(GetCurrentTextStyleClass());
}