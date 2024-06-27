// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/GameplayAbilities/Attributes/AccuracyModifiersAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/MoveUsageAttributes.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"

#include "BattleMoveFunctionCode.generated.h"

class AMoveAnimation;
class AMoveExecutor;
class IBattler;
class IBattleMove;
class IMove;

/**
 * The gameplay ability for using a move.
 */
UCLASS()
class POKEMONBATTLE_API UBattleMoveFunctionCode : public UGameplayAbility {
    GENERATED_BODY()

public:
    UBattleMoveFunctionCode();
    
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;
    bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, FGameplayTagContainer *OptionalRelevantTags) const override;
    void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

    void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
    static TArray<AActor *> FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo &ActorInfo);

    void OnFailureCheckComplete(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, bool bSuccess);
    
    void OnTargetValidationComplete(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, bool bSuccess);

    void CheckFailureOnNextTarget(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo, int32 Index = 0, bool bSuccess = false);

    bool AccuracyCheck(const UMoveUsageAttributes& MoveUsageAttributes, UAccuracyModifiersAttributeSet& AccuracyModifiers, const TScriptInterface<IBattler> &Target) const;

    void DamageCalculation(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo) const;

    UPROPERTY()
    TScriptInterface<IBattleMove> BattleMove;

    UPROPERTY()
    TScriptInterface<IBattler> User;

    UPROPERTY()
    TArray<TScriptInterface<IBattler>> Targets;

    UPROPERTY()
    TArray<TScriptInterface<IBattler>> ValidTargets;
    
    UPROPERTY()
    TObjectPtr<AMoveExecutor> Executor;

    UPROPERTY()
    TObjectPtr<AMoveAnimation> Animation;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageEffect;

    bool bSucceededAgainstTarget = false;
    
    /**
     * The attribute that is used to determine the amount of PP a move should be reduced by
     */
    UPROPERTY()
    FGameplayAttribute CostAttribute = UPokemonCoreAttributeSet::GetMoveCostAttribute();

};
