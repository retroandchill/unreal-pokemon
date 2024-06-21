// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/SelectableWidget.h"
#include "Data/RPGMenusSettings.h"
#include "Data/SelectionInputs.h"
#include "Primatives/SelectableOption.h"

USelectableWidget::USelectableWidget(const FObjectInitializer &ObjectInitializer) : UUserWidget(ObjectInitializer) {
    SetIsFocusable(true);

    auto Settings = GetDefault<URPGMenusSettings>();
    CursorSound = Settings->GetCursorSound();
    ConfirmSound = Settings->GetConfirmSound();
    CancelSound = Settings->GetCancelSound();
}

int32 USelectableWidget::GetItemCount_Implementation() const {
    return 0;
}

int32 USelectableWidget::GetRowCount() const {
    int32 ColumnCount = GetColumnCount();
    return (GetItemCount() + ColumnCount - 1) / ColumnCount;
}

int32 USelectableWidget::GetColumnCount_Implementation() const {
    return 1;
}

int32 USelectableWidget::GetIndex() const {
    return Index;
}

int32 USelectableWidget::GetRow(int32 IndexToCheck) const {
    return IndexToCheck / GetRowCount();
}

int32 USelectableWidget::GetColumn(int32 IndexToCheck) const {
    return IndexToCheck % GetRowCount();
}

void USelectableWidget::SetIndex(int32 NewIndex) {
    int32 OldIndex = Index;
    Index = FMath::Clamp(NewIndex, -1, GetItemCount() - 1);
    OnSelectionChange(OldIndex, Index);
}

void USelectableWidget::Deselect() {
    int32 OldIndex = Index;
    Index = -1;
    OnSelectionChange(OldIndex, Index);
}

bool USelectableWidget::IsActive() const {
    return bActive;
}

void USelectableWidget::SetActive(bool bNewActiveState) {
    if (bActive == bNewActiveState)
        return;

    bActive = bNewActiveState;
    OnActiveChanged(bActive);
}

FProcessConfirm &USelectableWidget::GetOnConfirm() {
    return OnConfirm;
}

FProcessCancel &USelectableWidget::GetOnCancel() {
    return OnCancel;
}

void USelectableWidget::NativeOnRemovedFromFocusPath(const FFocusEvent &InFocusEvent) {
    Super::NativeOnRemovedFromFocusPath(InFocusEvent);

    if (InFocusEvent.GetCause() == EFocusCause::Mouse && IsActive()) {
        SetKeyboardFocus();
    }
}

FReply USelectableWidget::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) {
    if (!IsActive() || InputMappings == nullptr)
        return FReply::Unhandled();

    auto Key = InKeyEvent.GetKey();
    if (auto CursorDirection = InputMappings->ParseDirectionalInputs(Key); CursorDirection.IsSet()) {
        PlaySound(CursorSound);
        ReceiveMoveCursor(CursorDirection.GetValue());
        return FReply::Handled();
    }

    if (InputMappings->IsConfirmInput(Key)) {
        int32 CurrentIndex = GetIndex();
        ConfirmOnIndex(CurrentIndex);
        return FReply::Handled();
    }

    if (InputMappings->IsCancelInput(Key)) {
        PlaySound(CancelSound);
        OnCancel.Broadcast();
        ProcessCancel();
        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void USelectableWidget::ConfirmOnIndex(int32 CurrentIndex) {
    PlaySound(ConfirmSound);
    OnConfirm.Broadcast(CurrentIndex);
    ProcessConfirm(CurrentIndex);
}

void USelectableWidget::ProcessClickedButton(USelectableOption *Option) {
    if (!IsActive()) {
        return;
    }

    int32 CurrentIndex = Option->GetOptionIndex();
    ConfirmOnIndex(CurrentIndex);
}

void USelectableWidget::ProcessHoveredButton(USelectableOption *Option) {
    if (!IsActive()) {
        return;
    }

    PlaySound(CursorSound);
    SetIndex(Option->GetOptionIndex());
}

void USelectableWidget::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::OnActiveChanged_Implementation(bool bNewActiveState) {
    if (bNewActiveState)
        SetKeyboardFocus();
}

void USelectableWidget::ProcessConfirm_Implementation(int32 CurrentIndex) {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::ProcessCancel_Implementation() {
    // No implementation, but we cannot have an abstract method in an Unreal class
}

int32 USelectableWidget::GetNextIndex_Implementation(ECursorDirection Direction) {
    int32 NewIndex = GetIndex();
    int32 ItemCount = GetItemCount();
    switch (Direction) {
        using enum ECursorDirection;
    case Up:
        if (GetRowCount() > 1) {
            NewIndex = bWrapSelection ? (ItemCount + NewIndex - GetColumnCount()) % ItemCount
                                      : FMath::Max(NewIndex - GetColumnCount(), 0);
        }
        break;
    case Down:
        if (GetRowCount() > 1) {
            NewIndex = bWrapSelection ? (NewIndex + GetColumnCount()) % ItemCount
                                      : FMath::Min(NewIndex + GetColumnCount(), ItemCount - 1);
        }
        break;
    case Left:
        if (GetColumnCount() > 1) {
            NewIndex = bWrapSelection ? (ItemCount + NewIndex - 1) % ItemCount : FMath::Max(NewIndex - 1, 0);
        }
        break;
    case Right:
        if (GetColumnCount() > 1) {
            NewIndex = bWrapSelection ? (NewIndex + 1) % ItemCount : FMath::Min(NewIndex + 1, ItemCount - 1);
        }
        break;
    }

    return NewIndex;
}

void USelectableWidget::ReceiveMoveCursor(ECursorDirection Direction) {
    if (!IsActive())
        return;

    SetIndex(GetNextIndex(Direction));
}
