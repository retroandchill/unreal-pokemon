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
    int32 GetOptionIndex() const;
    void SetOptionIndex(int32 NewOptionIndex);

    FProcessButtonAction &GetOnOptionClicked();

    FProcessButtonAction &GetOnOptionHovered();

  private:
    UFUNCTION()
    void OnClicked();

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

    UPROPERTY()
    FProcessButtonAction OnOptionClicked;

    UPROPERTY()
    FProcessButtonAction OnOptionHovered;
};
