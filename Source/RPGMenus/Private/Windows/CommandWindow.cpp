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
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Data/Command.h"
#include "Primatives/TextCommand.h"

UCommandWindow::UCommandWindow(const FObjectInitializer& ObjectInitializer) : USelectableWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UCommandWindow::RebuildWidget() {
	auto Original = USelectableWidget::RebuildWidget();
	AddCommands();
	OnSelectionChange(GetIndex());
	OnActiveChanged(IsActive());
	return Original;
}

void UCommandWindow::SynchronizeProperties() {
	RebuildWidget();
}

void UCommandWindow::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (GetIndex() >= GetItemCount()) {
		SetIndex(GetItemCount() - 1);
	}
}

int32 UCommandWindow::GetItemCount_Implementation() const {
	return ActiveCommands.Num();
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

	for (UCommand* const Command : Commands) {
		if (Command == nullptr || !Command->IsEnabled())
			continue;

		auto TextWidget = WidgetTree->ConstructWidget<UTextCommand>(DisplayTextWidgetClass);
		TextWidget->SetText(Command->GetText());

		int32 CurrentIndex = ActiveCommands.Num();
		auto Pos = GetCellPosition(CurrentIndex);
		CommandArea->AddChildToUniformGrid(TextWidget, Pos.Y, Pos.X);
		ActiveCommands.Add(Command);
		CommandWidgets.Add(TextWidget);
	}
}
