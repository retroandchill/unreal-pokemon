// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/HelpWindow.h"
#include "Primatives/DisplayText.h"

TSharedRef<SWidget> UHelpWindow::RebuildWidget() {
    auto Ret = Super::RebuildWidget();
    SetText(Text);
    return Ret;
}

void UHelpWindow::SynchronizeProperties() {
    Super::SynchronizeProperties();
    SetText(Text);
}

#if WITH_EDITOR
void UHelpWindow::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    SetText(Text);
}
#endif

const FText &UHelpWindow::GetText() const {
    return Text;
}

void UHelpWindow::SetText(const FText &NewText) {
    Text = NewText;
    if (HelpText != nullptr) {
        HelpText->SetText(Text);
    }
}
