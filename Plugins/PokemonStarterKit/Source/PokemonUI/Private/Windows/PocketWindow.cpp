// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/PocketWindow.h"
#include "Components/DisplayText.h"
#include "PokemonDataSettings.h"

FName UPocketWindow::GetCurrentPocket() const {
    return CurrentPocket;
}

void UPocketWindow::SetCurrentPocket(FName Pocket) {
    CurrentPocket = Pocket;
    UpdatePocketInfo();
}

void UPocketWindow::UpdatePocketInfo_Implementation() {
    auto &Settings = *GetDefault<UPokemonDataSettings>();
    const auto &[DisplayName, MaxPocketSize, bAutoSort] = Settings.PocketInfo.FindChecked(CurrentPocket);
    PocketName->SetText(DisplayName);
}