// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Animations/ProgressBarAnimation.h"
#include "Components/Common/PokemonInfoWidget.h"
#include "Pokemon/Breeding/PokemonGender.h"

#include "PokemonBattlePanel.generated.h"

class UEnhancedImage;
class UCommonTextStyle;
class UImage;
class IBattler;
class UProgressBar;
class UDisplayText;

/**
 * Dynamic delegate called when a progress bar is updated
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProgresBarUpdateComplete);

/**
 * The panel that displays the battle information about a Pokémon.
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UPokemonBattlePanel : public UUserWidget
{
    GENERATED_BODY()

  public:
    /**
     * Get the currently displayed battler
     * @return The battler whose information is being displayed
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<IBattler> &GetCurrentBattler() const
    {
        return CurrentBattler;
    }

    /**
     * Set the displayed battler
     * @param Battler The currently displayed battler
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Battle|Display")
    void SetBattler(const TScriptInterface<IBattler> &Battler);

    /**
     * Refresh the displayed information in the widget
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    void Refresh();

    /**
     * Animate the change in HP to the new value
     * @param MaxDuration The maximum duration allowed for the battler
     */
    UE5Coro::TCoroutine<> AnimateHP(float MaxDuration);

    /**
     * Refresh the displayed status condition of the contained Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    void RefreshStatusEffect();

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Display")
    void OnRefresh();

    /**
     * The function used to callback to the depleted HP.
     * @param NewPercent The new HP percentage
     */
    virtual void UpdateHPPercent(float NewPercent);

  private:
    /**
     * The widget that display's the HP percentage
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;

    /**
     * The widget that display's the status condition if the battler has a status condition
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEnhancedImage> StatusIcon;

    /**
     * The battler this widget displays the information for
     */
    UPROPERTY(BlueprintGetter = GetCurrentBattler, BlueprintSetter = SetBattler, Category = Battler)
    TScriptInterface<IBattler> CurrentBattler;
};
