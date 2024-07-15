// "Unreal Pokémon" created by Retro & Chill.

#include "Components/BattleMovePanel.h"
#include "Battle/Moves/BattleMove.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Components/DisplayText.h"
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
    return GraphicsSubsystem->GetTypePanelGraphic(Move->GetDisplayType());
}

void UBattleMovePanel::OnMoveSet() {
    using enum ESlateVisibility;
    check(Move != nullptr)
    MoveNameText->SetText(Move->GetDisplayName());
    MovePPText->SetText(FText::Format(MovePPFormat, {Move->GetCurrentPP(), Move->GetMaxPP()}));
    TypeIcon->SetVisibility(SelfHitTestInvisible);
    UWidgetUtilities::SetBrushFromAsset(TypeIcon, GetTypeIcon(), true);
}