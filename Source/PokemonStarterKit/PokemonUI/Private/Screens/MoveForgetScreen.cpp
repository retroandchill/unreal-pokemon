// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/MoveForgetScreen.h"
#include "Components/Common/MoveInfoWindow.h"
#include "Components/Common/MoveSelectWindow.h"
#include "Utilities/RPGMenuUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UMoveForgetScreen)

UMoveForgetScreen *UMoveForgetScreen::AddMoveForgetScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<UMoveForgetScreen>(WorldContextObject).GetPtrOrNull();
}

void UMoveForgetScreen::NativeConstruct() {
    Super::NativeConstruct();
    FOnMoveSelectionChanged::FDelegate Binding;
    Binding.BindDynamic(MoveInfoWindow, &UMoveInfoWindow::SetMove);
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

UE5Coro::TCoroutine<bool> UMoveForgetScreen::AwaitPlayerDecision() {
    auto [Result] = co_await OnMoveForgetComplete;
    co_return Result;
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