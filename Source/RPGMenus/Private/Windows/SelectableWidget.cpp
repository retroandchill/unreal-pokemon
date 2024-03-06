// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/SelectableWidget.h"

#include "Data/SelectionInputs.h"
#include "Kismet/GameplayStatics.h"

USelectableWidget::USelectableWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
	SetIsFocusable(true);
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

int32 USelectableWidget::GetRow(int32 IndexToCheck) {
	return IndexToCheck / GetRowCount();
}

int32 USelectableWidget::GetColumn(int32 IndexToCheck) {
	return IndexToCheck % GetRowCount();
}

void USelectableWidget::SetIndex(int32 NewIndex) {
	if (Index == NewIndex)
		return;

	int32 OldIndex = Index;
	Index = FMath::Clamp(NewIndex, -1, GetItemCount() - 1);
	OnSelectionChange(OldIndex, Index);
}

void USelectableWidget::Deselect() {
	if (Index == -1)
		return;
	
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

FReply USelectableWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	if (!IsActive() || InputMappings == nullptr)
		return FReply::Unhandled();
	
	bool bHandled = false;
	auto Key = InKeyEvent.GetKey();
	if (auto CursorDirection = InputMappings->ParseDirectionalInputs(Key); CursorDirection.IsSet()) {
		ReceiveMoveCursor(CursorDirection.GetValue());
	} else if (InputMappings->IsConfirmInput(Key)) {
		int32 CurrentIndex = GetIndex();
		OnConfirm.Broadcast(CurrentIndex);
		ProcessConfirm(CurrentIndex);
	} else if (InputMappings->IsCancelInput(Key)) {
		OnCancel.Broadcast();
		ProcessCancel();
	}
	
	
	return bHandled ? FReply::Handled() : FReply::Unhandled();
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

void USelectableWidget::NativeOnFocusLost(const FFocusEvent& InFocusEvent) {
	Super::NativeOnFocusLost(InFocusEvent);
	
	if (InFocusEvent.GetCause() == EFocusCause::Mouse) {
		SetKeyboardFocus();
	}
}

int32 USelectableWidget::GetNextIndex_Implementation(ECursorDirection Direction) {
	int32 NewIndex = GetIndex();
	int32 ItemCount = GetItemCount();
	switch (Direction) {
		using enum ECursorDirection;
	case Up:
		if (GetRowCount() > 1) {
			NewIndex = bWrapSelection ? (ItemCount + NewIndex - GetColumnCount()) % ItemCount : FMath::Max(NewIndex - GetColumnCount(), 0);
		}
		break;
	case Down:
		if (GetRowCount() > 1) {
			NewIndex = bWrapSelection ? (NewIndex + GetColumnCount()) % ItemCount : FMath::Min(NewIndex + GetColumnCount(), ItemCount - 1);
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
