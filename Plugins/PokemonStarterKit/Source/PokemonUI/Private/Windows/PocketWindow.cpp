// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PocketWindow.h"
#include "CommonTextBlock.h"
#include "Settings/BaseSettings.h"

FName UPocketWindow::GetCurrentPocket() const {
    return CurrentPocket;
}

void UPocketWindow::SetCurrentPocket(FName Pocket) {
    CurrentPocket = Pocket;
    UpdatePocketInfo();
}

void UPocketWindow::UpdatePocketInfo_Implementation() {
    auto &Settings = Pokemon::FBaseSettings::Get();
    const auto &[DisplayName, MaxPocketSize, bAutoSort] = Settings.GetPocketInfo().FindChecked(CurrentPocket);
    PocketName->SetText(DisplayName);
}