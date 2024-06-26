﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/SummaryPages.h"
#include "Components/WidgetSwitcher.h"
#include "Data/SelectionInputs.h"

void USummaryPages::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Pokemon, int32 PartyIndex) {
    CurrentPokemon = FPokemonIterator(Pokemon, PartyIndex);
    if (OnPokemonChange.IsBound()) {
        OnPokemonChange.Execute(*CurrentPokemon);
    }
}

FOnPokemonChange &USummaryPages::GetOnPokemonChange() {
    return OnPokemonChange;
}

const TScriptInterface<IPokemon> &USummaryPages::GetCurrentPokemon() const {
    return *CurrentPokemon;
}

UWidgetSwitcher *USummaryPages::GetPageSwitcher() const {
    return PageSwitcher;
}

int32 USummaryPages::GetItemCount_Implementation() const {
    return PageSwitcher->GetNumWidgets();
}

int32 USummaryPages::GetColumnCount_Implementation() const {
    return PageSwitcher->GetNumWidgets();
}

void USummaryPages::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    PageSwitcher->SetActiveWidgetIndex(NewIndex);
}

void USummaryPages::ReceiveMoveCursor(ECursorDirection Direction) {
    Super::ReceiveMoveCursor(Direction);
    if (!CurrentPokemon.CanCycle()) {
        return;
    }

    using enum ECursorDirection;
    bool bPokemonChanged = false;
    if (Direction == Up) {
        --CurrentPokemon;
        bPokemonChanged = true;
    } else if (Direction == Down) {
        ++CurrentPokemon;
        bPokemonChanged = true;
    }

    if (bPokemonChanged && OnPokemonChange.IsBound()) {
        OnPokemonChange.Execute(*CurrentPokemon);
    }
}