// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMenuOption.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"

void UBattleMenuOption::NativePreConstruct() {
    Super::NativePreConstruct();
    OptionText->SetText(Text);
    BackgroundImage->SetBrush(Image);
}

void UBattleMenuOption::NativeOnCurrentTextStyleChanged() {
    Super::NativeOnCurrentTextStyleChanged();
    if (OptionText != nullptr) {
        OptionText->SetTextStyle(GetCurrentTextStyleClass());
    }
}