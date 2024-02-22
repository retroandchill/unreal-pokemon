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

class USizeBox;
class UScrollBox;
class UCommand;
class UGridPanel;
class SUniformGridPanel;
class UMenuCommand;
class UUniformGridPanel;
class UTextCommand;

/**
 * Delegate for when the user presses confirm
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessCommand, int32, CurrentIndex, UCommand*, SelectedCommand);

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
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	int32 GetItemCount_Implementation() const override;

	/**
	 * Callback for when a command is selected
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection|Confirm")
	FProcessCommand OnCommandSelected;

protected:
	void OnSelectionChange_Implementation(int32 NewIndex) override;
	void ProcessConfirm_Implementation(int32 CurrentIndex) override;

private:
	
	/**
	 * Get the position of a particular cell in the grid based on the given index.
	 * @param TargetIndex The index in question to get the position of.
	 * @return The row and column of the cell.
	 */
	FIntVector2 GetCellPosition(int32 TargetIndex) const;


	/**
	 * Add the commands to the window.
	 */
	void AddCommands();
	
	/**
	 * The actual area where the window is drawn
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWindow> Window;

	/**
	 * The panel where all the commands and the cursor are placed
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> CommandArea;

	/**
	 * The scroll box used to contain the various command elements
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	/**
	 * The widget that acts as the cursor for the window
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidget> CursorWidget;

	/**
	 * The commands displayed in the window
	 */
	UPROPERTY(EditAnywhere, Instanced, Category = Commands)
	TArray<TObjectPtr<UCommand>> Commands;

	/**
	 * The widget type used for the display text shown to the player
	 */
	UPROPERTY(EditAnywhere, Category = Commands)
	TSubclassOf<UTextCommand> DisplayTextWidgetClass;

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
