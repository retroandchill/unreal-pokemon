// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "SwitchActionBase.generated.h"

class ITrainer;
class IBattler;
/**
 * Base gameplay ability class for the switch action in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API USwitchActionBase : public UGameplayAbility {
    GENERATED_BODY()

  public:
    /**
     * Default constructor.
     */
    USwitchActionBase();

    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                         const FGameplayAbilityActivationInfo ActivationInfo,
                         const FGameplayEventData *TriggerEventData) override;

  protected:
    /**
     * Play the animation to recall the Pokémon. It ends when {@link USwitchActionBase::SwapWithTarget} is called.
     * @param SwappingFrom The battler that is being recalled
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    void PlayRecallAnimation(const TScriptInterface<IBattler> &SwappingFrom);

    /**
     * Perform the internal swap of the Pokémon.
     */
    UFUNCTION(BlueprintCallable, Category = Switching)
    void SwapWithTarget();

    /**
     * Play the animation for sending the new battler out.
     * @param SwappingTo The battler that is being sent out
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Switching)
    void PlaySendOutAnimation(const TScriptInterface<IBattler> &SwappingTo);

    UFUNCTION(BlueprintCallable, Category = Switching)
    void TriggerOnSendOut();

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
