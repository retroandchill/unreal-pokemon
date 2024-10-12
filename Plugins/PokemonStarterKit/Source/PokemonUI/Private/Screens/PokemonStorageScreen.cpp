﻿// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/PokemonStorageScreen.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Storage/StorageBoxWindow.h"
#include "Components/Storage/StorageInfoPanel.h"
#include "Managers/PokemonSubsystem.h"
#include "Storage/StorageSystem.h"
#include "Pokemon/Pokemon.h"

void UPokemonStorageScreen::NativeConstruct() {
    Super::NativeConstruct();
    auto &StorageSystem = UPokemonSubsystem::GetInstance(this).GetStorageSystem();
    StorageBoxWindow->SetStorageBox(StorageSystem->GetCurrentBox());
    StorageBoxWindow->BindToOnSelectedPokemonChanged(this, &UPokemonStorageScreen::OnSelectedPokemonChanged);
}

void UPokemonStorageScreen::OnSelectedPokemonChanged(TOptional<IPokemon &> SelectedPokemon) {
    if (!SelectedPokemon.IsSet()) {
        StorageInfoPanel->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    StorageInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    StorageInfoPanel->SetSelectedPokemon(SelectedPokemon.GetInterface());
}