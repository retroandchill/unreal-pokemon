// "Unreal Pokémon" created by Retro & Chill.

#include "Components/MovePanel.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "Pokemon/Pokemon.h"
#include "Primatives/DisplayText.h"
#include "Utilities/WidgetUtilities.h"

const TScriptInterface<IMove> &UMovePanel::GetMove() {
    return Move;
}

void UMovePanel::SetMove(const TScriptInterface<IMove> &MoveIn) {
    Move = MoveIn;
    OnMoveSet();
}

bool UMovePanel::IsMoveToLearn() const {
    return bIsMoveToLearn;
}

void UMovePanel::SetIsMoveToLearn(bool bIsBeingLearned) {
    bIsMoveToLearn = bIsBeingLearned;
}

UObject *UMovePanel::GetTypeIcon_Implementation() {
    check(Move != nullptr)
    auto GraphicsSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    return GraphicsSubsystem->GetTypeIconGraphic(Move->GetType());
}

void UMovePanel::OnMoveSet() {
    using enum ESlateVisibility;

    if (Move != nullptr) {
        auto &MoveData = Move->GetMoveData();
        MoveNameText->SetText(MoveData.RealName);
        PPLabel->SetVisibility(SelfHitTestInvisible);
        MovePPText->SetText(FText::Format(MovePPFormat, {Move->GetCurrentPP(), MoveData.TotalPP}));
        TypeIcon->SetVisibility(SelfHitTestInvisible);
        UWidgetUtilities::SetBrushFromAsset(TypeIcon, GetTypeIcon(), true);
    } else {
        MoveNameText->SetText(UnknownMoveLabel);
        PPLabel->SetVisibility(Hidden);
        MovePPText->SetText(UnknownMovePP);
        TypeIcon->SetVisibility(Hidden);
    }
}