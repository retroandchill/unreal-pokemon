// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"

#include "BattleMoveFunctionCode.generated.h"

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

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Success Checks")
    TArray<AActor *> FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo &ActorInfo);

    
    UFUNCTION(BlueprintNativeEvent, Category = "Success Checks")
    bool MoveFailed(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

    UFUNCTION(BlueprintNativeEvent, Category = "Success Checks")
    bool MoveSucceedsAgainstTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount = 1);
    
private:
    UPROPERTY()
    TScriptInterface<IBattleMove> BattleMove;
    
    /**
     * The attribute that is used to determine the amount of PP a move should be reduced by
     */
    UPROPERTY()
    FGameplayAttribute CostAttribute = UPokemonCoreAttributeSet::GetMoveCostAttribute();

};
