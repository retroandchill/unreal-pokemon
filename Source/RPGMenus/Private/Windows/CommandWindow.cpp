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

#include "Components/GridSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Primatives/MenuCommand.h"
#include "Primatives/TextCommand.h"
#include "Widgets/Layout/SConstraintCanvas.h"

UCommandWindow::UCommandWindow(const FObjectInitializer& ObjectInitializer) : UWindow(ObjectInitializer) {
}

void UCommandWindow::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	ContentGrid.Reset();
}

void UCommandWindow::AddChildrenToSlots() {
	ContentGrid = SNew(SGridPanel);
	ContentGrid->SetColumnFill(0, 1);
	GetMyCanvas()->AddSlot().Anchors(FAnchors(0, 0, 1, 1)).AttachWidget(ContentGrid.ToSharedRef());
	
	for (auto PanelSlot : Slots) {
		if (auto TypedSlot = Cast<UGridSlot>(PanelSlot)) {
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(ContentGrid.ToSharedRef());
		}
	}
}

UClass* UCommandWindow::GetSlotClass() const {
	return UGridSlot::StaticClass();
}

void UCommandWindow::OnSlotAdded(UPanelSlot* InSlot) {
	// Add the child to the live canvas if it already exists
		auto GridSlot = CastChecked<UGridSlot>(InSlot);
		GridSlot->SetRow(Slots.Num() - 1);
	
	if (ContentGrid.IsValid()) {
		GridSlot->BuildSlot(ContentGrid.ToSharedRef());
	}
}

void UCommandWindow::OnSlotRemoved(UPanelSlot* InSlot) {
	// Remove the widget from the live slot if it exists.
	if (ContentGrid.IsValid() && InSlot->Content) {
		if (auto Widget = InSlot->Content->GetCachedWidget(); Widget.IsValid() ) {
			ContentGrid->RemoveSlot(Widget.ToSharedRef());
		}
	}
}
