// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

class IBattler;
struct FItem;

#include "BattleItemEffect.generated.h"

/**
 * Ability that handles the effect of the item being used in battle.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API UBattleItemEffect : public UGameplayAbility {
    GENERATED_BODY()

public:
    /**
     * Create the CDO for this class
     */
    UBattleItemEffect();

    /**
     * The item that is being used
     * @return The item that is being used
     */
    UFUNCTION(BlueprintPure, Category = Context)
    const FItem& GetItem() const;

    bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo,
                                     const FGameplayEventData *Payload) const override;
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                         const FGameplayAbilityActivationInfo ActivationInfo,
                         const FGameplayEventData *TriggerEventData) override;
    void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                    const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                    bool bWasCancelled) override;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Effect)
    void ApplyAbilityEffect(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);
    
    UFUNCTION(BlueprintCallable, Category = Finalization)
    void FinishAbility(bool bShouldConsumeItem = true);
    
    UFUNCTION(BlueprintNativeEvent, Category = Validation)
    bool IsTargetValid(const TScriptInterface<IBattler>& Battler);

private:
    /**
     * Filter out any invalid targets from this move
     * @param TriggerEventData
     * @return The array of filtered target actors
     */
    TArray<TScriptInterface<IBattler>> FilterInvalidTargets(
        const FGameplayEventData* TriggerEventData);
    
    FName ItemID;

};
