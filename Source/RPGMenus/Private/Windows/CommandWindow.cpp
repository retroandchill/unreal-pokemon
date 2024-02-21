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
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Data/Command.h"
#include "Fonts/FontMeasure.h"

UCommandWindow::UCommandWindow(const FObjectInitializer& ObjectInitializer) : USelectableWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UCommandWindow::RebuildWidget() {
	auto Original = USelectableWidget::RebuildWidget();

	ActiveCommands.Empty();
	CommandWidgets.Empty();
	if (CommandArea != nullptr) {
		CommandArea->ClearChildren();
		
		for (UCommand* const Command : Commands) {
			if (Command == nullptr || !Command->IsEnabled())
				continue;
		
			auto TextWidget = WidgetTree->ConstructWidget<UTextBlock>();
			TextWidget->SetText(Command->GetText());
			TextWidget->SetColorAndOpacity(Command->GetColorAndOpacity());
			TextWidget->SetFont(Command->GetFont());

			int32 CurrentIndex = ActiveCommands.Num();
			CommandArea->AddChildToGrid(TextWidget, CurrentIndex / GetColumnCount(), CurrentIndex % GetColumnCount());
			ActiveCommands.Add(Command);
			CommandWidgets.Add(TextWidget);
		}
	}
	
	return Original;
}

void UCommandWindow::SynchronizeProperties() {
	RebuildWidget();
}
