// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Animations/ProgressBarAnimation.h"
#include "CommonUserWidget.h"

#include "BattlerExpPanel.generated.h"

class UCommonTextBlock;
class UCommonNumericTextBlock;
class IBattler;
class UProgressBar;

/**
 *
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattlerExpPanel : public UCommonUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Sets the battler information to be displayed on the experience panel.
     *
     * @param Battler The battler interface to be displayed.
     * @param Level Optional parameter to set the level of the battler. If not provided, the battler's current level is
     * used.
     * @param ExpGainPercent Optional parameter to set the experience gain percentage. If not provided, the battler's
     * current experience percentage is used.
     */
    void SetBattler(const TScriptInterface<IBattler> &Battler, const TOptional<int32> &Level = TOptional<int32>(),
                    const TOptional<float> &ExpGainPercent = TOptional<float>());

    /**
     * Updates the experience gain display on the experience panel.
     *
     * @param Gain The amount of experience gain to be displayed.
     */
    void ChangeExpGainDisplay(int32 Gain);

    /**
     * Animates the experience gain on the experience panel over a specified duration.
     *
     * @param MaxDuration The maximum duration allowed for the animation, in seconds.
     */
    UE5Coro::TCoroutine<> AnimateGain(float MaxDuration = 3.f);

    /**
     * Binds a callback to be executed when the experience gain animation completes.
     *
     * @param Callback The delegate to be invoked upon the completion of the animation.
     */
    void BindOnAnimationComplete(FSimpleDelegate Callback);

  protected:
    /**
     * Blueprint event called when the battler information is set.
     *
     * @param Battler The battler interface whose information is being set.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void OnBattlerSet(const TScriptInterface<IBattler> &Battler);

    /**
     * Blueprint event that is called to display level-up visual effects or notifications.
     * This method can be implemented in Blueprint to customize the behavior when a battler levels up.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void DisplayLevelUp();

  private:
    void UpdateExpBarPercent(float NewPercent);
    void OnLevelUp();
    void OnExpGainComplete() const;

    UPROPERTY()
    TScriptInterface<IBattler> CurrentBattler;

    int32 DisplayedLevel = 0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonNumericTextBlock> LevelText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> ExpBar;

    int32 ExpGain = 0;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> ExpGainText;

    FSimpleMulticastDelegate OnGainAnimationComplete;
};
