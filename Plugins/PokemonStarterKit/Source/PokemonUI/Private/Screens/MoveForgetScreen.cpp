﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/MoveForgetScreen.h"
#include "Windows/MoveInfoWindow.h"
#include "Windows/MoveSelectWindow.h"

void UMoveForgetScreen::NativeConstruct() {
    Super::NativeConstruct();
    FOnMoveSelectionChanged::FDelegate Binding;
    Binding.BindDynamic(MoveInfoWindow, &UMoveInfoWindow::RefreshMove);
    MoveSelectWindow->BindToOnMoveSelectionChanged(Binding);
    MoveSelectWindow->GetOnConfirm().AddUniqueDynamic(this, &UMoveForgetScreen::OnMoveSelected);
    MoveSelectWindow->GetOnCancel().AddUniqueDynamic(this, &UMoveForgetScreen::OnCanceled);
}

void UMoveForgetScreen::InitializeScene(const TScriptInterface<IPokemon> &Pokemon) {
    OwnedPokemon = Pokemon;
    RefreshLayout(OwnedPokemon, false);
    MoveSelectWindow->DisplayMoves(Pokemon);
    MoveSelectWindow->ActivateWidget();
    MoveSelectWindow->SetIndex(0);
}

void UMoveForgetScreen::InitializeScene(const TScriptInterface<IPokemon> &Pokemon, FName Move) {
    OwnedPokemon = Pokemon;
    MoveToLearn.Emplace(Move);
    RefreshLayout(OwnedPokemon, true);
    MoveSelectWindow->BeginMoveLearnDisplay(Pokemon, Move);
    MoveSelectWindow->ActivateWidget();
    MoveSelectWindow->SetIndex(0);
}

void UMoveForgetScreen::BindToOnMoveForgetComplete(FOnMoveForgetComplete::FDelegate &&Callback) {
    OnMoveForgetComplete.Add(MoveTemp(Callback));
}

void UMoveForgetScreen::MoveForgetComplete(bool bMoveForgotten) {
    CloseScreen();
    OnMoveForgetComplete.Broadcast(bMoveForgotten);
}

UMoveSelectWindow *UMoveForgetScreen::GetMoveSelectWindow() const {
    return MoveSelectWindow;
}

UMoveInfoWindow *UMoveForgetScreen::GetMoveInfoWindow() const {
    return MoveInfoWindow;
}

void UMoveForgetScreen::OnMoveSelected(int32 Index) {
    if (MoveToLearn.IsSet()) {
        if (Index == MoveSelectWindow->GetItemCount() - 1) {
            PromptGiveUpLearn(OwnedPokemon, *MoveToLearn);
        } else {
            PromptMoveLearn(OwnedPokemon, Index, *MoveToLearn);
        }
    } else {
        PromptMoveForget(OwnedPokemon, Index);
    }
}

void UMoveForgetScreen::OnCanceled() {
    if (MoveToLearn.IsSet()) {
        PromptGiveUpLearn(OwnedPokemon, *MoveToLearn);
    } else {
        PromptGiveUpForget(OwnedPokemon);
    }
}