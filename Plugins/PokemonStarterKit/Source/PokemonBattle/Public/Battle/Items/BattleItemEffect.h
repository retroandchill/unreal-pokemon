// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include <range/v3/view/any_view.hpp>

struct FRunningMessageSet;
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
    /**
     * Apply the global (no targets) effect of the item.
     * @param User The user of the item.
     * @param Messages The running message (and animations) to display to the player for this usage
     * @return Was the effect successfully applied? If none of these checks return true, this item will not be consumed.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Effect)
    bool ApplyGlobalEffect(const TScriptInterface<IBattler>& User, const FRunningMessageSet &Messages);

    /**
     * Apply the effect of the item to an individual target.
     * @param User The user of the item.
     * @param Target The target of the item in question
     * @param Messages The running message (and animations) to display to the player for this usage
     * @return Was the effect successfully applied? If none of these checks return true, this item will not be consumed.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Effect)
    bool ApplyEffectToTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet &Messages);

    /**
     * Display the results of the items usage.
     * @param Messages The messages to display to the player.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void DisplayResults(const FRunningMessageSet &Messages);

    /**
     * Check if an individual target for the item is valid
     * @param Battler Was the item target valid
     * @return Is this a valid target for the move.
     */
    UFUNCTION(BlueprintNativeEvent, Category = Validation)
    bool IsTargetValid(const TScriptInterface<IBattler>& Battler);

private:
    /**
     * Filter out any invalid targets from this move
     * @param TriggerEventData
     * @return The array of filtered target actors
     */
    ranges::any_view<TScriptInterface<IBattler>> FilterInvalidTargets(
        const FGameplayEventData* TriggerEventData);
    
    FName ItemID;

    bool bShouldConsumeItem = false;

};
