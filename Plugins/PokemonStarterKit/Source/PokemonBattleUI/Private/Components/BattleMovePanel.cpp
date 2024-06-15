// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMovePanel.h"
#include "Battle/Moves/BattleMove.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Primatives/DisplayText.h"
#include "Utilities/WidgetUtilities.h"

const TScriptInterface<IBattleMove> &UBattleMovePanel::GetMove() const {
    return Move;
}

void UBattleMovePanel::SetMove(const TScriptInterface<IBattleMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet();
}

UObject *UBattleMovePanel::GetTypeIcon_Implementation() {
    check(Move != nullptr)
    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    return GraphicsSubsystem->GetTypePanelGraphic(IBattleMove::Execute_GetDisplayType(Move.GetObject()));
}

void UBattleMovePanel::OnMoveSet() {
    using enum ESlateVisibility;
    check(Move != nullptr)
    auto MoveObject = Move.GetObject();
    MoveNameText->SetText(IBattleMove::Execute_GetDisplayName(MoveObject));
    MovePPText->SetText(FText::Format(
        MovePPFormat, {IBattleMove::Execute_GetCurrentPP(MoveObject), IBattleMove::Execute_GetMaxPP(MoveObject)}));
    TypeIcon->SetVisibility(SelfHitTestInvisible);
    UWidgetUtilities::SetBrushFromAsset(TypeIcon, GetTypeIcon(), true);
}