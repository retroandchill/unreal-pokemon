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
#include "Screens/Screen.h"

#include "RPGPlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Windows/SelectableWidget.h"

UScreen::UScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	SetIsFocusable(true);
}

TSharedRef<SWidget> UScreen::RebuildWidget() {
	auto Ret = Super::RebuildWidget();

	SelectableWidgets.Empty();
	WidgetTree->ForEachWidget([this](UWidget *Widget) {
		auto SelectableWidget = Cast<USelectableWidget>(Widget);
		if (SelectableWidget == nullptr)
			return;

		SelectableWidgets.Emplace(SelectableWidget);
	});
	
	return Ret;
}

void UScreen::CloseScreen() {
	auto RPGPlayerController = Cast<ARPGPlayerController>(GetOwningPlayer());
	if (RPGPlayerController == nullptr)
		return;

	RPGPlayerController->RemoveScreenFromStack();
}
