// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SelectableOption.generated.h"

class UButton;

/**
 * Delegate for when the button is hovered
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProcessButtonAction, USelectableOption *, Option);

/**
 * Represents a selectable option within a widget. Contains a button that will set the index of the menu to the given
 * option.
 */
UCLASS(Abstract)
class RPGMENUS_API USelectableOption : public UUserWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
    /**
     * Get the index of this option in the menu.
     * @return The index of this option
     */
    UFUNCTION(BlueprintPure, Category = Selelection)
    int32 GetOptionIndex() const;

    /**
     * Set the index of this option in the menu.
     * @param NewOptionIndex The index of this option
     */
    UFUNCTION(BlueprintCallable, Category = Selection)
    void SetOptionIndex(int32 NewOptionIndex);

    /**
     * Get the click delegate.
     * @return Delegate dispatched when this option is clicked on
     */
    FProcessButtonAction &GetOnOptionClicked();

    /**
     * Get the hover delegate.
     * @return Delegate dispatched when this option is hovered over
     */
    FProcessButtonAction &GetOnOptionHovered();

  private:
    /**
     * Callback when this option is clicked
     */
    UFUNCTION()
    void OnClicked();

    /**
     * Callback for when this option is hovered by the mouse
     */
    UFUNCTION()
    void OnHovered();

    /**
     * The actual button that the player can click on
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PrimaryButton;

    /**
     * The index for when the player hovers over this option.
     */
    int32 OptionIndex;

    /**
     * Delegate dispatched when this option is clicked on
     */
    UPROPERTY(BlueprintAssignable, Category = "Selection|Events")
    FProcessButtonAction OnOptionClicked;

    /**
     * Delegate dispatched when this option is hovered over
     */
    UPROPERTY(BlueprintAssignable, Category = "Selection|Events")
    FProcessButtonAction OnOptionHovered;
};
