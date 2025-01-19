// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RetroLib/Ranges/Views/Generator.h"
#include "UE5CoroGAS.h"

class IBattler;
struct FItem;

#include "BattleItemEffect.generated.h"

/**
 * Ability that handles the effect of the item being used in battle.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UBattleItemEffect : public UUE5CoroGameplayAbility {
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
    const FItem &GetItem() const;

    bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo,
                                     const FGameplayEventData *Payload) const override;

  protected:
    UE5Coro::GAS::FAbilityCoroutine ExecuteAbility(FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo *ActorInfo,
                                                   FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData *TriggerEventData) override;

    /**
     * Apply the global (no targets) effect of the item.
     * @param User The user of the item.
     * @param
     * @return Was the effect successfully applied? If none of these checks return true, this item will not be consumed.
     */
    virtual UE5Coro::TCoroutine<bool> ApplyGlobalEffect(const TScriptInterface<IBattler> &User,
                                                        FForceLatentCoroutine = {});

    /**
     * Apply the effect of the item to an individual target.
     * @param User The user of the item.
     * @param Target The target of the item in question
     * @param
     * @return Was the effect successfully applied? If none of these checks return true, this item will not be consumed.
     */
    virtual UE5Coro::TCoroutine<bool> ApplyEffectToTarget(const TScriptInterface<IBattler> &User,
                                                          const TScriptInterface<IBattler> &Target,
                                                          FForceLatentCoroutine = {});

    /**
     * Check if an individual target for the item is valid
     * @param Battler Was the item target valid
     * @param
     * @return Is this a valid target for the move.
     */
    virtual UE5Coro::TCoroutine<bool> IsTargetValid(const TScriptInterface<IBattler> &Battler,
                                                    FForceLatentCoroutine = {});

  private:
    FName ItemID;

    bool bShouldConsumeItem = false;
};
