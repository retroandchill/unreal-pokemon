// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UE5CoroGAS.h"

#include "SwitchAction.generated.h"

class UBattleAnimationGetter;
class ITrainer;
class IBattler;
class IPokemonCoroutineDispatcher;
/**
 * Base gameplay ability class for the switch action in battle.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API USwitchAction : public UUE5CoroGameplayAbility
{
    GENERATED_BODY()

  public:
    /**
     * Default constructor.
     */
    USwitchAction();

  protected:
    UE5Coro::GAS::FAbilityCoroutine ExecuteAbility(FGameplayAbilitySpecHandle Handle,
                                                   const FGameplayAbilityActorInfo *ActorInfo,
                                                   FGameplayAbilityActivationInfo ActivationInfo,
                                                   const FGameplayEventData *TriggerEventData) override;

    /**
     * Perform the internal swap of the Pokémon.
     */
    UE5Coro::TCoroutine<> SwapWithTarget(const TScriptInterface<IBattler> &SwappingFrom,
                                         UBattleAnimationGetter &AnimationGetter,
                                         IPokemonCoroutineDispatcher &Dispatcher);

    UE5Coro::TCoroutine<> TriggerOnSendOut();

  private:
    UPROPERTY()
    TScriptInterface<ITrainer> OwningTrainer;
    int32 UserIndex = 0;
    int32 SwitchTargetIndex = 0;

    UPROPERTY()
    TScriptInterface<IBattler> SwapTarget;
};
