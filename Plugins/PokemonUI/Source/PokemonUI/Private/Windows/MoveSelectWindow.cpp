// "Unreal Pokémon" created by Retro & Chill.

#include "Windows/MoveSelectWindow.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Components/MovePanel.h"
#include "Pokemon/Moves/MoveBlock.h"
#include "Pokemon/Pokemon.h"
#include "Settings/BaseSettings.h"

void UMoveSelectWindow::DisplayMoves(const TScriptInterface<IPokemon> &Pokemon) {
    CurrentPokemon = Pokemon;

    Algo::ForEach(MovePanels, &UWidget::RemoveFromParent);
    int32 MoveCount = Pokemon::FBaseSettings::Get().GetMaxMoves();
    auto Moves = Pokemon->GetMoveBlock()->GetMoves();

    MovePanels.Empty();
    check(PanelClass != nullptr)
    for (int i = 0; i < MoveCount; i++) {
        MovePanels.Emplace(CreateMovePanel(i < Moves.Num() ? Moves[i] : nullptr));
    }
}

int32 UMoveSelectWindow::GetItemCount_Implementation() const {
    return CurrentPokemon->GetMoveBlock()->GetMoves().Num();
}

void UMoveSelectWindow::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    Super::OnSelectionChange_Implementation(OldIndex, NewIndex);
    if (NewIndex == -1) {
        CursorWidget->SetVisibility(ESlateVisibility::Hidden);
    } else {
        CursorWidget->SetVisibility(ESlateVisibility::Visible);
        SetCursorPosition(CursorWidget, NewIndex);
        OnMoveSelectionChanged.Broadcast(MovePanels[NewIndex]->GetMove());
    }
}

UMovePanel *UMoveSelectWindow::CreateMovePanel(const TScriptInterface<IMove> &Move) {
    check(PanelClass != nullptr)
    auto Panel = WidgetTree->ConstructWidget(PanelClass);
    Panel->SetMove(Move);
    SlotPanel(Panel);
    Panel->GetOnOptionClicked().AddDynamic(this, &UMoveSelectWindow::ProcessClickedButton);
    Panel->GetOnOptionHovered().AddDynamic(this, &UMoveSelectWindow::ProcessHoveredButton);
    return Panel;
}