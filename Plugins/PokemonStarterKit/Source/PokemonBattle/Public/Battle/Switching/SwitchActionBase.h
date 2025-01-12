// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UE5CoroGAS.h"

#include "SwitchActionBase.generated.h"

class ITrainer;
class IBattler;
/**
 * Base gameplay ability class for the switch action in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API USwitchActionBase : public UUE5CoroGameplayAbility {
    GENERATED_BODY()

  public:
    /**
     * Default constructor.
     */
    USwitchActionBase();

  protected:
    UE5Coro::GAS::FAbilityCoroutine ExecuteAbility(FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo *ActorInfo,
                                                   FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData *TriggerEventData) override;

    /**
     * Play the animation to recall the Pokémon. It ends when {@link USwitchActionBase::SwapWithTarget} is called.
     * @param SwappingFrom The battler that is being recalled
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    void QueueRecallAnimation(const TScriptInterface<IBattler> &SwappingFrom);

    /**
     * Perform the internal swap of the Pokémon.
     */
    UE5Coro::TCoroutine<> SwapWithTarget();

    /**
     * Play the animation for sending the new battler out.
     * @param SwappingTo The battler that is being sent out
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    void QueueSendOutAnimation(const TScriptInterface<IBattler> &SwappingTo);

    UE5Coro::TCoroutine<> TriggerOnSendOut();

    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    void DisplaySwitchInEffects(const TScriptInterface<IBattler> &Battler);

  private:
    UPROPERTY()
    TScriptInterface<ITrainer> OwningTrainer;
    int32 UserIndex = 0;
    int32 SwitchTargetIndex = 0;

    UPROPERTY()
    TScriptInterface<IBattler> SwapTarget;
};
