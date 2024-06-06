// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PocketWindow.h"
#include "Primatives/DisplayText.h"
#include "Settings/BagSettings.h"

FName UPocketWindow::GetCurrentPocket() const {
    return CurrentPocket;
}

void UPocketWindow::SetCurrentPocket(FName Pocket) {
    CurrentPocket = Pocket;
    UpdatePocketInfo();
}

void UPocketWindow::UpdatePocketInfo_Implementation() {
    auto &PocketInfo = GetDefault<UBagSettings>()->GetPocketInfo().FindChecked(CurrentPocket);
    PocketName->SetText(PocketInfo.DisplayName);
}