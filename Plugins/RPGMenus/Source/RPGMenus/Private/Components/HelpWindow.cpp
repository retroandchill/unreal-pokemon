// "Unreal Pokémon" created by Retro & Chill.

#include "Components/HelpWindow.h"
#include "Components/DisplayText.h"

void UHelpWindow::NativePreConstruct() {
    Super::NativePreConstruct();
    SetText(Text);
}

const FText &UHelpWindow::GetText() const {
    return Text;
}

void UHelpWindow::SetText(FText NewText) {
    Text = NewText;
    if (HelpText != nullptr) {
        HelpText->SetText(Text);
    }
}
