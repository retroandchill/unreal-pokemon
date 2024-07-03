// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Animations/ProgressBarAnimation.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "PokemonUI/Public/Components/PokemonInfoWidget.h"
#include "Text/TextColor.h"

#include "PokemonBattlePanel.generated.h"

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
class POKEMONBATTLEUI_API UPokemonBattlePanel : public UUserWidget {
    GENERATED_BODY()

protected:
    void NativeConstruct() override;
    
  public:
    /**
     * Get the currently displayed battler
     * @return The battler whose information is being displayed
     */
    const TScriptInterface<IBattler> &GetCurrentBattler() const;
    
    /**
     * Set the displayed battler
     * @param Battler The currently displayed battler
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    void SetBattler(const TScriptInterface<IBattler> &Battler);

    /**
     * Refresh the displayed information in the widget
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    virtual void Refresh();

    /**
     * Animate the change in HP to the new value
     * @param MaxDuration The maximum duration allowed for the battler
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Display")
    void AnimateHP(float MaxDuration);

    /**
     * Get the delegate used for updating the progress bar
     * @param Binding The desired action 
     */
    void BindToOnProgressBarUpdateComplete(const FOnProgresBarUpdateComplete::FDelegate& Binding);

    /**
     * Unbind all dynamic delegates from the HP update callback
     * @param Object The object to unbind the delegate sfor
     */
    UFUNCTION()
    void UnbindAllHPUpdateDelegates(UObject* Object);

protected:
    /**
     * The function used to callback to the depleted HP.
     * @param NewPercent The new HP percentage
     */
    virtual void UpdateHPPercent(float NewPercent);

    /**
     * Called when the HP percentage update is complete
     */
    virtual void HPPercentUpdateComplete() const;

private:
    /**
     * The widget to display the Pokémon's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonName;

    /**
     * The widget to display the Pokémon's gender
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonGender;

    /**
     * The widget to display the Pokémon's level
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokemonLevel;

    /**
     * The widget that display's the HP percentage
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HPBar;

    /**
     * The widget that display's the status condition if the battler has a status condition
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> StatusIcon;

    /**
     * The battler this widget displays the information for
     */
    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    /**
     * The color settings for the gender text widget
     */
    UPROPERTY(EditAnywhere, Category = "Visuals|Text")
    TMap<EPokemonGender, FTextColor> GenderTextColors;

    /**
     * The animation for updating the HP bar
     */
    Pokemon::UI::FProgressBarAnimation HPBarUpdateAnimation;

    /**
     * Callback for when the HP bar is finished animating
     */
    UPROPERTY()
    FOnProgresBarUpdateComplete OnHPBarUpdated;
};
