// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PocketWindow.h"
#include "Components/DisplayText.h"
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
    auto &PocketInfo = Settings.GetPocketInfo().FindChecked(CurrentPocket);
    PocketName->SetText(PocketInfo.DisplayName);
}