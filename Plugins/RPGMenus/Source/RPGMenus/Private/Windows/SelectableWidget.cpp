// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/SelectableWidget.h"
#include "CommonButtonBase.h"
#include "Algo/ForEach.h"

USelectableWidget::USelectableWidget(const FObjectInitializer &Initializer) : UCommonActivatableWidget(Initializer) {
    bIsBackHandler = true;
    bIsBackActionDisplayedInActionBar = true;
}

int32 USelectableWidget::GetItemCount() const {
    return SelectableButtons.Num();
}

int32 USelectableWidget::GetIndex() const {
    return Index;
}

void USelectableWidget::SetIndex(int32 NewIndex) {
    int32 OldIndex = Index;
    Index = FMath::Clamp(NewIndex, INDEX_NONE, GetItemCount() - 1);
    OnSelectionChange(OldIndex, Index);
}

void USelectableWidget::Deselect() {
    int32 OldIndex = Index;
    Index = INDEX_NONE;
    OnSelectionChange(OldIndex, Index);
}

UCommonButtonBase * USelectableWidget::GetSelectedOption() const {
    if (!SelectableButtons.IsValidIndex(Index)) {
        return nullptr;
    }

    return SelectableButtons[Index];
}

FProcessConfirm &USelectableWidget::GetOnConfirm() {
    return OnConfirm;
}

FProcessCancel &USelectableWidget::GetOnCancel() {
    return OnCancel;
}

UWidget * USelectableWidget::NativeGetDesiredFocusTarget() const {
    if (!SelectableButtons.IsValidIndex(Index)) {
        return nullptr;
    }
    
    return SelectableButtons[Index];
}

void USelectableWidget::NativeOnActivated() {
    Super::NativeOnActivated();
    if (GetVisibility() == ESlateVisibility::HitTestInvisible) {
        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void USelectableWidget::NativeOnDeactivated() {
    Super::NativeOnDeactivated();
    if (IsVisible()) {
        SetVisibility(ESlateVisibility::HitTestInvisible);
    }
}

void USelectableWidget::ConfirmOnIndex(int32 CurrentIndex) {
    OnConfirm.Broadcast(CurrentIndex);
    ProcessConfirm(CurrentIndex);
}

void USelectableWidget::Cancel() {
    OnCancel.Broadcast();
    ProcessCancel();
}

bool USelectableWidget::NativeOnHandleBackAction() {
    Cancel();
    return true;
}

UCommonButtonBase * USelectableWidget::GetSelectableOption(int32 OptionIndex) const {
    if (!SelectableButtons.IsValidIndex(OptionIndex)) {
        return nullptr;
    }

    return SelectableButtons[OptionIndex];
}

void USelectableWidget::ClearSelectableOptions() {
    Algo::ForEach(SelectableButtons, &UWidget::RemoveFromParent);
    SelectableButtons.Reset();
}

void USelectableWidget::SlotOption(UCommonButtonBase *Option, int32 OptionIndex) {
    SelectableButtons.Emplace(Option);
    Option->OnClicked().AddWeakLambda(this, [this, OptionIndex] {
       ConfirmOnIndex(OptionIndex); 
    });
    Option->OnHovered().AddWeakLambda(this, [this, OptionIndex] {
       SetIndex(OptionIndex); 
    });
    PlaceOptionIntoWidget(Option, OptionIndex);
}

void USelectableWidget::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessConfirm_Implementation(int32 CurrentIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessCancel_Implementation() {
    // No implementation, but we cannot have an abstract method in an Unreal class
}
