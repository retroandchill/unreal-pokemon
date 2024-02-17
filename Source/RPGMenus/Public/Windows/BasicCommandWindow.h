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
#include "SelectableWindow.h"
#include "BasicCommandWindow.generated.h"

class UMenuCommand;
class UUniformGridPanel;
class UTextCommand;

/**
 * Basic command window, with a set of commands that can be dispatched
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UBasicCommandWindow : public USelectableWindow {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the window
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UBasicCommandWindow(const FObjectInitializer& ObjectInitializer);
	
	TSharedRef<SWidget> RebuildWidget() override;
	int32 GetItemCount_Implementation() const override;

private:
	/**
	 * The list of commands in the menu
	 */
	UPROPERTY(EditAnywhere, Instanced, Category = Commands)
	TArray<TObjectPtr<UMenuCommand>> Commands;

	/**
	 * The class used to instantiate the command widgets in the array
	 */
	UPROPERTY(EditAnywhere, Category = Commands)
	TSubclassOf<UTextCommand> CommandWidgetClass;

	/**
	 * The list of command widgets added to the menu
	 */
	UPROPERTY(EditAnywhere, Instanced, Category = Commands)
	TArray<TObjectPtr<UTextCommand>> CommandWidgets;
	
	/**
	 * The panel where the commands are placed
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ContentsPanel;
};
