// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/MoveSelectWindow.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Common/MovePanel.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "PokemonDataSettings.h"

void UMoveSelectWindow::DisplayMoves(const TScriptInterface<IPokemon> &Pokemon) {
    CurrentPokemon = Pokemon;

    ClearSelectableOptions();
    int32 MoveCount = GetDefault<UPokemonDataSettings>()->MaxMoves;
    auto Moves = Pokemon->GetMoveBlock()->GetMoves();

    check(PanelClass != nullptr)
    for (int i = 0; i < MoveCount; i++) {
        CreateMovePanel(i < Moves.Num() ? Moves[i] : nullptr);
    }

    if (MoveToLearn.IsSet()) {
        auto TempMove = CurrentPokemon->GetMoveBlock()->CreateNewMove({.Move = *MoveToLearn });
        CreateMovePanel(TempMove, true);
    }
}

void UMoveSelectWindow::BeginMoveLearnDisplay(const TScriptInterface<IPokemon> &Pokemon, FName Move) {
    MoveToLearn.Emplace(Move);
    DisplayMoves(Pokemon);
}

void UMoveSelectWindow::BindToOnMoveSelectionChanged(const FOnMoveSelectionChanged::FDelegate &Callback) {
    OnMoveSelectionChanged.Add(Callback);
}

void UMoveSelectWindow::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    if (NewIndex != INDEX_NONE) {
        OnMoveSelectionChanged.Broadcast(GetSelectableOption<UMovePanel>(NewIndex)->GetMove());
    }
}

UMovePanel *UMoveSelectWindow::CreateMovePanel(const TScriptInterface<IMove> &Move, bool bIsMoveToLearn) {
    check(PanelClass != nullptr)
    auto Panel = WidgetTree->ConstructWidget(PanelClass);
    Panel->SetMove(Move);
    Panel->SetIsMoveToLearn(bIsMoveToLearn);
    SlotOption(Panel);
    return Panel;
}