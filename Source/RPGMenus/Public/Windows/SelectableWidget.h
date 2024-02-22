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
#include "Window.h"
#include "Blueprint/UserWidget.h"
#include "Data/CursorDirection.h"
#include "SelectableWidget.generated.h"

class USelectionInputs;
struct FInputActionInstance;
class UInputMappingContext;
class UInputAction;

/**
 * Delegate for when the user presses confirm
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProcessConfirm, int32, CurrentIndex);

/**
 * Delegate for when the user presses cancel
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProcessCancel);

/**
 * Widget for a menu that options can be selected from using the cursor
 */
UCLASS(BlueprintType)
class RPGMENUS_API USelectableWidget : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the widget
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit USelectableWidget(const FObjectInitializer &ObjectInitializer);
	
	/**
	 * Get the number of items in the menu that can be selected
	 * @return The total number of items
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Selection)
	int32 GetItemCount() const;

	/**
	 * Get the number of rows in the menu
	 * @return The total number of items
	 */
	UFUNCTION(BlueprintPure, Category = Selection)
	int32 GetRowCount() const;

	/**
	 * Get the number of columns in the menu
	 * @return The total number of items
	 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Selection)
	int32 GetColumnCount() const;
	
	/**
	 * Get the current index of the menu 
	 * @return The current selection index of the menu (-1 = inactive)
	 */
	UFUNCTION(BlueprintPure, Category = Selection)
	int32 GetIndex() const;

	/**
	 * Set the current index of the menu
	 * @param NewIndex The current selection index of the menu
	 */
	UFUNCTION(BlueprintCallable, Category = Selection)
	void SetIndex(int32 NewIndex);

	/**
	 * End selection for the current menu
	 */
	UFUNCTION(BlueprintCallable, Category = Selection)
	void Deselect();

	/**
	 * Is this menu actively selectable
	 * @return Can the player change selections in the menu
	 */
	UFUNCTION(BlueprintPure, Category = Selection)
	bool IsActive() const;

	/**
	 * Set the selectability status of the menu
	 * @param bNewActiveState The new selectability status
	 */
	UFUNCTION(BlueprintCallable, Category = Selection)
	void SetActive(bool bNewActiveState);

	/**
	 * The delegate bound to confirm
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection|Confirm")
	FProcessConfirm OnConfirm;

	/**
	 * The delegate bound to cancel
	 */
	UPROPERTY(BlueprintAssignable, Category = "Selection|Cancel")
	FProcessCancel OnCancel;
	
protected:
	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	
	/**
	 * Called when the selection is changed
	 * @param NewIndex The new index to select to
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Selection)
	void OnSelectionChange(int32 NewIndex);

	/**
	 * Called when the active state is changed
	 * @param bNewActiveState The new active state of the widget
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Selection)
	void OnActiveChanged(bool bNewActiveState);

	/**
	 * Additional functionality for when confirm is selected
	 * @param CurrentIndex The current index of the window
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Selection|Confirm")
	void ProcessConfirm(int32 CurrentIndex);

	/**
	 * Additional functionality for when cancel is selected
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Selection|Cancel")
	void ProcessCancel();

	void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	
private:
	/**
	 * Function called when the cursor is moved
	 * @param Direction The received cursor input
	 */
	void ReceiveMoveCursor(ECursorDirection Direction);
	
	/**
	 * The index of the menu in question
	 */
	UPROPERTY(EditAnywhere, Category = Selection)
	int32 Index = -1;

	/**
	 * Is this menu actively selectable
	 */
	UPROPERTY(EditAnywhere, Category = Selection)
	bool bActive = false;

	/**
	 * Do the selection options wrap when input would exceed the end
	 */
	UPROPERTY(EditAnywhere, Category = Selection)
	bool bWrapSelection = true;

	/**
	 * Do the selection options wrap when input would exceed the end
	 */
	UPROPERTY(EditAnywhere, Category = Selection)
	TObjectPtr<USelectionInputs> InputMappings;
	
};
