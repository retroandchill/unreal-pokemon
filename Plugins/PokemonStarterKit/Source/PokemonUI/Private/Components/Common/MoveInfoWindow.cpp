// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/MoveInfoWindow.h"
#include "Components/DisplayText.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "Utilities/WidgetUtilities.h"

void UMoveInfoWindow::RefreshMove_Implementation(const TScriptInterface<IMove> &Move) {
    auto &MoveData = Move->GetMoveData();
    UWidgetUtilities::SetBrushFromAsset(CategoryGraphic, CategoryGraphics.FindChecked(MoveData.Category), true);
    PowerText->SetText(GetMoveDamageText(MoveData));
    AccuracyText->SetText(GetMoveAccuracyText(MoveData));
    DescriptionText->SetText(MoveData.Description);
}

FText UMoveInfoWindow::GetMoveDamageText_Implementation(const FMoveData &MoveData) const {
    if (MoveData.Category == EMoveDamageCategory::Status) {
        return FText::FromStringView(TEXT("---"));
    }

    if (MoveData.Power == FMoveData::VARIABLE_POWER) {
        return FText::FromStringView(TEXT("???"));
    }

    return FText::FromString(FString::FromInt(MoveData.Power));
}

FText UMoveInfoWindow::GetMoveAccuracyText_Implementation(const FMoveData &MoveData) const {
    if (MoveData.Accuracy == 0) {
        return FText::FromStringView(TEXT("---"));
    }

    return FText::FromString(FString::Format(TEXT("{0}%"), {MoveData.Accuracy}));
}