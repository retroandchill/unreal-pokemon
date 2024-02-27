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
#include "Windows/CommandWindow.h"

#include "Blueprint/WidgetTree.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/Command.h"
#include "Primatives/DisplayText.h"

UCommandWindow::UCommandWindow(const FObjectInitializer& ObjectInitializer) : USelectableWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UCommandWindow::RebuildWidget() {
	auto Original = USelectableWidget::RebuildWidget();
	AddCommands();
	OnSelectionChange(GetIndex());
	OnActiveChanged(IsActive());
	SetScrollArrowsVisible();
	return Original;
}

void UCommandWindow::SynchronizeProperties() {
	RebuildWidget();
}

void UCommandWindow::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	// If the commands are set but the active command are not, then we don't want to change anything
	if (!Commands.IsEmpty() && ActiveCommands.IsEmpty())
		return;
	
	if (GetIndex() >= GetItemCount()) {
		SetIndex(GetItemCount() - 1);
	}
}

void UCommandWindow::NativeConstruct() {
	Super::NativeConstruct();
	SetScrollArrowsVisible();
}

void UCommandWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (auto VisibleLines = GetPageMax(); VisibleLines.IsSet()) {
		int32 Row = GetIndex() / GetColumnCount();
		int32 NewTopRow = Row - (VisibleLines.GetValue() - 1) / 2;
		NewTopRow = FMath::Max(FMath::Min(NewTopRow, GetRowCount() - VisibleLines.GetValue()), 0);
		if (TopRow != NewTopRow) {
			TopRow = NewTopRow;
			ScrollBox->SetScrollOffset(TopRow * CommandHeight.GetValue());
		}
	}
	
	SetScrollArrowsVisible();
}

int32 UCommandWindow::GetItemCount_Implementation() const {
	return ActiveCommands.Num();
}

TOptional<int32> UCommandWindow::GetPageMax() {
	if (ScrollBox == nullptr)
		return TOptional<int32>();
	
	auto ScrollBoxGeometry = ScrollBox->GetCachedGeometry();
	float Height = ScrollBoxGeometry.GetLocalSize().Y;
	if (FMath::IsNearlyZero(Height) || !CommandHeight.IsSet())
		return true;
	
	int32 VisibleLines = FMath::FloorToInt(Height / CommandHeight.GetValue());
	if (bOverride_MaxLines) {
		VisibleLines = FMath::Max(VisibleLines, MaxLines);
	}
	return VisibleLines;
}

void UCommandWindow::OnSelectionChange_Implementation(int32 NewIndex) {
	if (CursorWidget == nullptr)
		return;
	
	auto CursorSlot = Cast<UUniformGridSlot>(CursorWidget->Slot);
	if (CursorSlot == nullptr)
		return;

	if (NewIndex >= 0) {
		auto Pos = GetCellPosition(NewIndex);
		CursorSlot->SetColumn(Pos.X);
		CursorSlot->SetRow(Pos.Y);
		CursorWidget->SetVisibility(ESlateVisibility::Visible);
	} else {
		CursorWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCommandWindow::ProcessConfirm_Implementation(int32 CurrentIndex) {
	if (CurrentIndex >= ActiveCommands.Num())
		return;
	
	UCommand *CurrentCommand = ActiveCommands[CurrentIndex];
	OnCommandSelected.Broadcast(CurrentIndex, CurrentCommand);
}

FIntVector2 UCommandWindow::GetCellPosition(int32 TargetIndex) const {
	int32 ColumnCount = GetColumnCount();
	return FIntVector2(TargetIndex % ColumnCount, TargetIndex / ColumnCount);
}

void UCommandWindow::AddCommands() {
	ActiveCommands.Empty();
	if (CommandArea == nullptr || DisplayTextWidgetClass == nullptr)
		return;
	
	for (UWidget* Command : CommandWidgets) {
		CommandArea->RemoveChild(Command);
	}
	CommandWidgets.Empty();

	CommandHeight.Reset();
	for (UCommand* const Command : Commands) {
		if (Command == nullptr || !Command->IsEnabled())
			continue;

		auto TextWidget = WidgetTree->ConstructWidget<UDisplayText>(DisplayTextWidgetClass);
		TextWidget->SetText(Command->GetText());

		int32 CurrentIndex = ActiveCommands.Num();
		auto Pos = GetCellPosition(CurrentIndex);
		CommandArea->AddChildToUniformGrid(TextWidget, Pos.Y, Pos.X);
		ActiveCommands.Add(Command);
		CommandWidgets.Add(TextWidget);

		if (!CommandHeight.IsSet()) {
			auto TextPadding = TextWidget->GetDisplayTextPadding();
			CommandHeight.Emplace(TextWidget->GetTextSize().Y + TextPadding.Top + TextPadding.Bottom);
		}
	}

	if (SizeBox == nullptr)
		return;

	if (CommandHeight.IsSet() && bOverride_MaxLines) {
		SizeBox->SetMaxDesiredHeight(CommandHeight.GetValue() * MaxLines);
	} else {
		SizeBox->ClearMaxDesiredHeight();
	}

}

void UCommandWindow::SetScrollArrowsVisible() {
	if (ScrollBox == nullptr)
		return;

	if (TopRow > 0) {
		UpArrow->SetVisibility(ESlateVisibility::Visible);
	} else {
		UpArrow->SetVisibility(ESlateVisibility::Collapsed);
	}

	int32 RowCount = GetRowCount();
	int32 PageMax = GetPageMax().GetValue();
	if (TopRow + PageMax < RowCount && RowCount > PageMax) {
		DownArrow->SetVisibility(ESlateVisibility::Visible);
	} else {
		DownArrow->SetVisibility(ESlateVisibility::Collapsed);
	}
}
