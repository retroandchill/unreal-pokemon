//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Windows/SelectableWidget.h"

#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
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

void USelectableWidget::SetIndex(int32 NewIndex) {
	if (Index == NewIndex)
		return;
	
	Index = FMath::Clamp(NewIndex, -1, GetItemCount() - 1);
	OnSelectionChange(Index);
}

void USelectableWidget::Deselect() {
	if (Index == -1)
		return;
	
	Index = -1;
	OnSelectionChange(Index);
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
	auto CursorDirection = InputMappings->ParseInputs(Key);\
	if (CursorDirection.IsSet()) {
		ReceiveMoveCursor(CursorDirection.GetValue());
	}
	
	return bHandled ? FReply::Handled() : FReply::Unhandled();
}

void USelectableWidget::OnSelectionChange_Implementation(int32 NewIndex) {
	// No implementation, but we cannot have an abstract method in an Unreal class
}

void USelectableWidget::OnActiveChanged_Implementation(bool bNewActiveState) {
	// No implementation, but we cannot have an abstract method in an Unreal class
	if (bNewActiveState)
		SetFocus();
}


void USelectableWidget::ReceiveMoveCursor(ECursorDirection Direction) {
	if (!IsActive())
		return;

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

	SetIndex(NewIndex);
}
