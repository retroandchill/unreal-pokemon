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
#include "Windows/BasicCommandWindow.h"

#include "Components/UniformGridPanel.h"
#include "Primatives/MenuCommand.h"
#include "Primatives/TextCommand.h"

UBasicCommandWindow::UBasicCommandWindow(const FObjectInitializer& ObjectInitializer) : USelectableWindow(ObjectInitializer) {
}

TSharedRef<SWidget> UBasicCommandWindow::RebuildWidget() {
	auto Ret = Super::RebuildWidget();

	if (ContentsPanel != nullptr && CommandWidgetClass != nullptr) {
		for (auto Widget : CommandWidgets) {
			ContentsPanel->RemoveChild(Widget);
		}
		CommandWidgets.Empty();

		int32 ColumnCount = GetColumnCount();
		for (int32 i = 0; i < Commands.Num(); i++) {
			UMenuCommand *Command = Commands[i];
			
			auto NewWidget = NewObject<UTextCommand>(GetTransientPackage(), CommandWidgetClass);
			NewWidget->SetCommand(Command);
			ContentsPanel->AddChildToUniformGrid(NewWidget, i / ColumnCount, i % ColumnCount);
			CommandWidgets.Add(NewWidget);
		}
	}
	
	return Ret;
}

int32 UBasicCommandWindow::GetItemCount_Implementation() const {
	return Commands.Num();
}
