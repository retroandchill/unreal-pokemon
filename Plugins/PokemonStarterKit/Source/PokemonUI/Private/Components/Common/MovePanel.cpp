// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/MovePanel.h"
#include "Components/DisplayText.h"
#include "Components/Image.h"
#include "Graphics/AssetClasses.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "Ranges/Optional/GetPtrOrNull.h"
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
    // clang-format off
    return Pokemon::Assets::Graphics::TypeIcons.LoadAsset(Move->GetType()) |
        UE::Optionals::FlatMap([](const FImageAsset& Asset) { return Asset.TryGet(); }) |
        UE::Optionals::GetPtrOrNull;
    // clang-format on
}

void UMovePanel::OnMoveSet() {
    using enum ESlateVisibility;

    if (Move != nullptr) {
        auto &MoveData = Move->GetMoveData();
        MoveNameText->SetText(MoveData.RealName);
        MovePPText->SetText(FText::Format(MovePPFormat, {Move->GetCurrentPP(), MoveData.TotalPP}));
        TypeIcon->SetVisibility(SelfHitTestInvisible);
        UWidgetUtilities::SetBrushFromAsset(TypeIcon, GetTypeIcon(), true);
    } else {
        MoveNameText->SetText(UnknownMoveLabel);
        MovePPText->SetText(UnknownMovePP);
        TypeIcon->SetVisibility(Hidden);
    }
}