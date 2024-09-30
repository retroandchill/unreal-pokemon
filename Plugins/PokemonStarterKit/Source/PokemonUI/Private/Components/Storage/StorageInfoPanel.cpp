// "Unreal Pokémon" created by Retro & Chill.


#include "Components/Storage/StorageInfoPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Storage/StorageInfoPage.h"

void UStorageInfoPanel::SetSelectedPokemon_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    CurrentPokemon = Pokemon;
    auto CurrentWidget = StorageInfoSwitcher->GetActiveWidget();
    check(CurrentWidget->Implements<UStorageInfoPage>());
    IStorageInfoPage::Execute_DisplayInfo(CurrentWidget, Pokemon);
}