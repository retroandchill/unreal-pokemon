// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "PlayBattlerHPAnimation.generated.h"

class IBattler;
/**
 * Delegate played when the HP animation is complete
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHPAnimationComplete);

/**
 * Action callback used to unbind an action from a widget
 */
DECLARE_DELEGATE(FUnbindActions)

/**
 * Play a battler's HP draining animation
 */
UCLASS(DisplayName = "Play Battler HP Animation", meta = (HideThen))
class POKEMONBATTLEUI_API UPlayBattlerHPAnimation : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Open the menu to select a Pokémon from the party
     * @param WorldContextObject The object used to obtain the state of the world
     * @param Battler The battler whose HP is being updated
     * @param MaxDuration The maximum duration for the animation
     * @return The node to execute the task with
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Play Battler HP Animation",
              meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"),
              Category = "Battle|Animations")
    static UPlayBattlerHPAnimation *PlayBattlerHPAnimation(const UObject *WorldContextObject,
                                                           const TScriptInterface<IBattler> &Battler,
                                                           float MaxDuration = 1.f);

protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;

  private:
    /**
     * Called when animation is complete
     */
    UPROPERTY(BlueprintAssignable)
    FHPAnimationComplete OnSelected;

    /**
     * Delegate used for unbinding actions
     */
    FUnbindActions UnbindActions;

    /**
     * The battler whose HP is being updated
     */
    UPROPERTY()
    TScriptInterface<IBattler> Battler;

    /**
     * The maximum duration for the animation
     */
    UPROPERTY()
    float MaxDuration;
};
