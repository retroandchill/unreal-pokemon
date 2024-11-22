// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "Ranges/Casting/DynamicCast.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"

#include "SelectableWidget.generated.h"

class UCommonButtonGroupBase;
class UCommonButtonBase;
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
 * @class USelectableWidget
 *
 * @brief A base class for selectable widgets in RPG menus.
 */
UCLASS(BlueprintType)
class RPGMENUS_API USelectableWidget : public UCommonActivatableWidget {
    GENERATED_BODY()

  public:
    explicit USelectableWidget(const FObjectInitializer &Initializer);

  protected:
    void NativePreConstruct() override;

  public:
    /**
     * Get the number of items in the menu that can be selected
     * @return The total number of items
     */
    UFUNCTION(BlueprintPure, Category = Selection)
    int32 GetItemCount() const;

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

    UFUNCTION(BlueprintPure, Category = Selection)
    UCommonButtonBase *GetSelectedOption() const;

    /**
     * @brief Retrieves the reference to the delegate for when the user presses confirm.
     *
     * @details This method returns a reference to the FProcessConfirm delegate for when the user presses confirm
     *          in the SelectableWidget. The delegate can be used to register callback functions to be executed
     *          when the confirm event occurs.
     *
     * @return The reference to the FProcessConfirm delegate for when the user presses confirm.
     */
    FProcessConfirm &GetOnConfirm();

    /**
     * Retrieves the delegate for when the user cancels.
     *
     * @return The delegate for when the user cancels.
     */
    FProcessCancel &GetOnCancel();

  protected:
    UWidget *NativeGetDesiredFocusTarget() const override;
    void NativeOnActivated() override;
    void NativeOnDeactivated() override;

    /**
     * A convenience method to handle additional functionality when the user confirms a selection based on the specified
     * index.
     *
     * @param CurrentIndex The current index of the menu
     */
    void ConfirmOnIndex(int32 CurrentIndex);

    /**
     * Method used to invoke the cancel operation for this widget
     */
    void Cancel();

    bool NativeOnHandleBackAction() override;

    /**
     * Called when the selection is changed
     * @param OldIndex The previous index of this widget
     * @param NewIndex The new index to select to
     */
    UFUNCTION(BlueprintNativeEvent, Category = Selection)
    void OnSelectionChange(int32 OldIndex, int32 NewIndex);

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
     * Get the full list of selectable options for this widget
     * @tparam T The type of option to expect
     * @return A view of the options that will cast to the given type
     */
    template <typename T>
    auto GetSelectableOptions() const {
        // clang-format off
        return SelectableButtons |
               UE::Ranges::Map(UE::Ranges::DynamicCastChecked<T>);
        // clang-format on
    }

    /**
     * Get the selectable option for this widget of the given type
     * @tparam T The type of widget to get
     * @param OptionIndex The index of the option to look for
     * @return The found widget
     */
    template <typename T>
    T *GetSelectableOption(int32 OptionIndex) const {
        if (!SelectableButtons.IsValidIndex(OptionIndex)) {
            return nullptr;
        }

        return CastChecked<T>(GetSelectableOption(OptionIndex));
    }

    /**
     * Get the selectable option for this widget
     * @param OptionIndex The index of the option to look for
     * @return The found widget
     */
    UFUNCTION(BlueprintPure, Category = "Selection|Options")
    UCommonButtonBase *GetSelectableOption(int32 OptionIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Selection|Options")
    void ClearSelectableOptions(bool bRemoveWidgets = true);

    /**
     * Slot an option into the widget
     * @param Option The option to be slotted
     */
    void SlotOption(UCommonButtonBase *Option);

    UFUNCTION(BlueprintCallable, Category = "Selection|Options")
    int32 AddOptionToWidget(UCommonButtonBase *Option);

    /**
     * The actual method called to slot the option into the UMG widget
     * @param Option The option to be slotted
     * @param OptionIndex The index of the option to slot
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Selection|Options")
    void PlaceOptionIntoWidget(UWidget *Option, int32 OptionIndex);

  private:
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
    int32 Index = INDEX_NONE;

    /**
     * Should options be selection when the mouse hovers over it?
     */
    UPROPERTY(EditAnywhere, Category = Selection)
    bool bSelectOptionOnHover = true;

    /**
     * The child buttons that are part of this object
     */
    UPROPERTY()
    TArray<TObjectPtr<UCommonButtonBase>> SelectableButtons;

    UPROPERTY()
    TObjectPtr<UCommonButtonGroupBase> SelectableButtonGroup;
};
