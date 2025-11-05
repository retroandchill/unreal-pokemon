// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"

#include "RPGButtonBase.generated.h"

/**
 * Specialized button base that has the ability to be disabled/hidden by the game as needed.
 */
UCLASS(Abstract, DisplayName = "RPG Button Base")
class RPGMENUS_API URPGButtonBase : public UCommonButtonBase {
    GENERATED_BODY()

  public:
    URPGButtonBase();

  protected:
    void NativePreConstruct() override;

  public:
    /**
     * Updates the visibility of the button based on its current state. This method checks
     * whether the button should be visible or not, taking into account both the default
     * visibility state and any delegates that determine visibility. If the button is
     * not visible, it sets the button's visibility to the `DisabledVisibility` state. Otherwise,
     * it sets the button's visibility to the `EnabledVisibility` state.
     */
    void UpdateVisibility();

  private:
    /**
     * Is this option visible to the player?
     */
    UPROPERTY(EditAnywhere, Category = Visibility)
    bool bIsVisible = true;

    UPROPERTY()
    FGetBool bIsVisibleDelegate;

    /**
     * What should the visibility be for this option if it is marked as visible
     */
    UPROPERTY(EditAnywhere, Category = Visibility, meta = (InvalidEnumValues = "Collapsed, Hidden"))
    ESlateVisibility EnabledVisibility = ESlateVisibility::SelfHitTestInvisible;

    /**
     * What should the visibility be for this option if it is marked as invisible
     */
    UPROPERTY(EditAnywhere, Category = Visibility, meta = (ValidEnumValues = "Collapsed, Hidden"))
    ESlateVisibility DisabledVisibility = ESlateVisibility::Collapsed;
};
