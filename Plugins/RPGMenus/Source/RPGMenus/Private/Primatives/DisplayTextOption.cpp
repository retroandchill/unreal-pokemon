// "Unreal Pokémon" created by Retro & Chill.

#include "Primatives/DisplayTextOption.h"
#include "CommonTextBlock.h"

void UDisplayTextOption::SetText(const FText &NewText) {
    DisplayText->SetText(NewText);
}

void UDisplayTextOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    DisplayText->SetStyle(GetCurrentTextStyleClass());
}