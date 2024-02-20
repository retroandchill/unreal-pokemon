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
#pragma once

#include "CoreMinimal.h"
#include "SelectableWidget.h"
#include "CommandWindow.generated.h"

class UCommand;
class UGridPanel;
class SUniformGridPanel;
class UMenuCommand;
class UUniformGridPanel;
class UTextCommand;

/**
 * Basic command window, with a set of commands that can be dispatched
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UCommandWindow : public USelectableWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UCommandWindow(const FObjectInitializer& ObjectInitializer);
	
	TSharedRef<SWidget> RebuildWidget() override;
	void SynchronizeProperties() override;

private:
	/**
	 * The actual area where the window is drawn
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWindow> Window;

	/**
	 * The panel where all the commands and the cursor are placed
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> CommandArea;

	/**
	 * The commands displayed in the window
	 */
	UPROPERTY(EditAnywhere, Instanced, Category = Commands)
	TArray<TObjectPtr<UCommand>> Commands;

	/**
	 * The internal list of active commands
	 */
	UPROPERTY()
	TArray<TObjectPtr<UCommand>> ActiveCommands;

	/**
	 * The actual widgets that make up the commands
	 */
	UPROPERTY()
	TArray<TObjectPtr<UWidget>> CommandWidgets;

};
