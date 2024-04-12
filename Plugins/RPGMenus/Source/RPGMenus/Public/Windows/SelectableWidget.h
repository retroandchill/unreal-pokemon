// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Data/CursorDirection.h"
#include "SelectableWidget.generated.h"

class USelectableOption;
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
     * Convenience method to get the row of a given index.
     * @param IndexToCheck The index to get the row of.
     * @return The row that index resides in.
     */
    UFUNCTION(BlueprintPure, Category = "Selection|Utilities")
    int32 GetRow(int32 IndexToCheck) const;

    /**
     * Convenience method to get the row of a given index.
     * @param IndexToCheck The index to get the row of.
     * @return The row that index resides in.
     */
    UFUNCTION(BlueprintPure, Category = "Selection|Utilities")
    int32 GetColumn(int32 IndexToCheck) const;

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

  protected:
    void NativeOnRemovedFromFocusPath(const FFocusEvent &InFocusEvent) override;
    
    FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override;

    /**
     * A convenience method to handle additional functionality when the user confirms a selection based on the specified
     * index. This method plays the ConfirmSound, triggers the OnConfirm event, and invokes the ProcessConfirm method.
     *
     * @param CurrentIndex The current index of the menu
     */
    void ConfirmOnIndex(int32 CurrentIndex);
    
    /**
     * Process the clicked button event for the Command Window
     *
     * @param Option The selectable option that was clicked
     */
    UFUNCTION()
    void ProcessClickedButton(USelectableOption* Option);

    /**
     * Process the hovered button event for the Command Window
     *
     * @param Option The selectable option that was hovered
     */
    UFUNCTION()
    void ProcessHoveredButton(USelectableOption* Option);
    
    /**
     * Called when the selection is changed
     * @param OldIndex The previous index of this widget
     * @param NewIndex The new index to select to
     */
    UFUNCTION(BlueprintNativeEvent, Category = Selection)
    void OnSelectionChange(int32 OldIndex, int32 NewIndex);

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

    /**
     * Process the procedure for handling when the cursor moves
     * @param Direction The direction the cursor should move in
     */
    UFUNCTION(BlueprintNativeEvent, Category = Selection)
    int32 GetNextIndex(ECursorDirection Direction);

  private:
    /**
     * Function called when the cursor is moved
     * @param Direction The received cursor input
     */
    void ReceiveMoveCursor(ECursorDirection Direction);

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

    /**
     * The sound played when the cursor moves
     */
    UPROPERTY(EditAnywhere, Category = Sound)
    TObjectPtr<USoundBase> CursorSound;

    /**
     * The sound played when the player confirms a choice
     */
    UPROPERTY(EditAnywhere, Category = Sound)
    TObjectPtr<USoundBase> ConfirmSound;

    /**
     * The sound played when the player cancels on selection
     */
    UPROPERTY(EditAnywhere, Category = Sound)
    TObjectPtr<USoundBase> CancelSound;
};
