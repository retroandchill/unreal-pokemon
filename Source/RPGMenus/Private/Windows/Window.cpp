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
#include "Windows/Window.h"

#include "Components/CanvasPanelSlot.h"
#include "Windows/Windowskin.h"

UWindow::UWindow(const FObjectInitializer& ObjectInitializer) : UPanelWidget(ObjectInitializer) {
	Brush.DrawAs = ESlateBrushDrawType::Box;
}

TSharedRef<SWidget> UWindow::RebuildWidget() {
	MyCanvas = SNew(SConstraintCanvas);
	
	auto Overlay = SNew(SOverlay);
	Overlay->AddSlot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).AttachWidget(SNew(SImage).Image(&Brush));
	FMargin Padding(0, 0, 0, 0);
	if (Windowskin != nullptr) {
		Padding = Windowskin->GetMargins();
	}
	Overlay->AddSlot().HAlign(HAlign_Fill).VAlign(VAlign_Fill).Padding(Padding).AttachWidget(MyCanvas.ToSharedRef());
	AddChildrenToSlots();
	return Overlay;
}

void UWindow::SynchronizeProperties() {
	if (Windowskin != nullptr) {
		Brush.TintColor = FSlateColor(FColor(255, 255, 255));

		auto SourceTexture = Windowskin->GetSourceTexture();
		auto &Margins = Windowskin->GetMargins();
		double TextureWidth = SourceTexture->GetSizeX();
		double TextureHeight = SourceTexture->GetSizeY();
		
		Brush.SetResourceObject(SourceTexture);
		Brush.Margin = FMargin(Margins.Left / TextureWidth, Margins.Top / TextureHeight,
			Margins.Right / TextureWidth, Margins.Bottom / TextureHeight);
	} else {
		Brush.TintColor = FSlateColor(FColor(0, 0, 0, 0));
		Brush.SetResourceObject(nullptr);
	}
}

void UWindow::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);
	MyCanvas.Reset();
}

void UWindow::AddChildrenToSlots() {
	for (auto PanelSlot : Slots) {
		if (auto TypedSlot = Cast<UCanvasPanelSlot>(PanelSlot)) {
			TypedSlot->Parent = this;
			TypedSlot->BuildSlot(MyCanvas.ToSharedRef());
		}
	}
}

UWindowskin* UWindow::GetWindowskin() const {
	return Windowskin;
}

TSharedPtr<SConstraintCanvas>& UWindow::GetMyCanvas() {
	return MyCanvas;
}

UClass* UWindow::GetSlotClass() const {
	return UCanvasPanelSlot::StaticClass();
}

void UWindow::OnSlotAdded(UPanelSlot* InSlot) {
	// Add the child to the live canvas if it already exists
	if ( MyCanvas.IsValid() ) {
		CastChecked<UCanvasPanelSlot>(InSlot)->BuildSlot(MyCanvas.ToSharedRef());
	}
}

void UWindow::OnSlotRemoved(UPanelSlot* InSlot) {
	// Remove the widget from the live slot if it exists.
	if ( MyCanvas.IsValid() && InSlot->Content) {
		if (auto Widget = InSlot->Content->GetCachedWidget(); Widget.IsValid() ) {
			MyCanvas->RemoveSlot(Widget.ToSharedRef());
		}
	}
}
