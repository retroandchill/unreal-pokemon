// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "InteractiveButtonGroup.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

#include "SelectableWidget.generated.h"

class UInteractiveButtonGroup;
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
class INTERACTIVEUI_API USelectableWidget : public UCommonActivatableWidget
{
    GENERATED_BODY()

  protected:
    void NativePreConstruct() override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UInteractiveButtonGroup* GetButtons() const
    {
        return Buttons.Get();   
    }

public:
    UWidget* NativeGetDesiredFocusTarget() const override;

    const TOptional<int32>& GetDesiredFocusIndex() const
    {
        return DesiredFocusIndex;
    }

    UFUNCTION(BlueprintPure, Category = "Selection")
    int32 GetItemCount() const
    {
        return Buttons->GetButtonCount();
    }
    
    UFUNCTION(BlueprintCallable, Category = "Selection")
    bool TryGetDesiredFocusIndex(int32& OutIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SetDesiredFocusIndex(int32 Index);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ClearDesiredFocusIndex()
    {
        DesiredFocusIndex.Reset();
    }

    /**
     * Get the current index of the menu
     * @return The current selection index of the menu (-1 = inactive)
     */
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    UFUNCTION(BlueprintPure, Category = Selection)
    int32 GetIndex() const;

    /**
     * Set the current index of the menu
     * @param NewIndex The current selection index of the menu
     */
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    UFUNCTION(BlueprintCallable, Category = Selection)
    void SetIndex(int32 NewIndex);

    /**
     * End selection for the current menu
    */
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
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
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    FProcessConfirm &GetOnConfirm()
    {
        return OnConfirm;   
    }

    /**
     * Retrieves the delegate for when the user cancels.
     *
     * @return The delegate for when the user cancels.
     */
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    FProcessCancel &GetOnCancel()
    {
        return OnCancel;  
    }

protected:
    /**
     * Get the full list of selectable options for this widget
     * @tparam T The type of option to expect
     * @return A view of the options that will cast to the given type
     */
    template <typename T>
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    auto GetSelectableOptions() const
    {
        // clang-format off
        return Retro::Ranges::Views::Iota(0) |
                Retro::Ranges::Views::Take(GetButtons()->GetButtonCount()) |
                Retro::Ranges::Views::Transform([this](const int32 Index) { return CastChecked<T>(GetSelectableOption(Index)); });
        // clang-format on
    }

    /**
     * Get the selectable option for this widget of the given type
     * @tparam T The type of widget to get
     * @param OptionIndex The index of the option to look for
     * @return The found widget
     */
    template <typename T>
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    T *GetSelectableOption(const int32 OptionIndex) const
    {
        return CastChecked<T>(GetSelectableOption(OptionIndex));
    }

    /**
     * Get the selectable option for this widget
     * @param OptionIndex The index of the option to look for
     * @return The found widget
     */
    UFUNCTION(BlueprintPure, Category = "Selection|Options")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    UCommonButtonBase *GetSelectableOption(int32 OptionIndex) const;

    UFUNCTION(BlueprintCallable, Category = "Selection|Options")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void ClearSelectableOptions(bool bRemoveWidgets = true);

    /**
     * Slot an option into the widget
     * @param Option The option to be slotted
     */
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void SlotOption(UCommonButtonBase *Option);

    UFUNCTION(BlueprintCallable, Category = "Selection|Options")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    int32 AddOptionToWidget(UCommonButtonBase *Option);

    /**
     * The actual method called to slot the option into the UMG widget
     * @param Option The option to be slotted
     * @param OptionIndex The index of the option to slot
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Selection|Options")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void PlaceOptionIntoWidget(UWidget *Option, int32 OptionIndex);
    
    /**
     * Called when the selection is changed
     * @param OldIndex The previous index of this widget
     * @param NewIndex The new index to select to
     */
    UFUNCTION(BlueprintNativeEvent, Category = Selection)
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void OnSelectionChange(int32 OldIndex, int32 NewIndex);

    /**
     * Additional functionality for when confirm is selected
     * @param CurrentIndex The current index of the window
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Selection|Confirm")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void ProcessConfirm(int32 CurrentIndex);

    /**
     * Additional functionality for when cancel is selected
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Selection|Cancel")
    UE_DEPRECATED(0.2, "This is just here so things can build for now")
    void ProcessCancel();

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
    
    UPROPERTY(BlueprintGetter = GetButtons, Category = "Selection")
    TObjectPtr<UInteractiveButtonGroup> Buttons;

    UPROPERTY(EditAnywhere, Getter, Category = "Selection", meta = (ClampMin = 0, UIMin = 0))
    TOptional<int32> DesiredFocusIndex;
};
