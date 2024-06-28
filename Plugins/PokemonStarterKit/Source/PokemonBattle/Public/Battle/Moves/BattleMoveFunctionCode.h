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

USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FRunningMessageSet {
    GENERATED_BODY()

    TSharedRef<TArray<FText>> Messages = MakeShared<TArray<FText>>();
    
};

/**
 * The gameplay ability for using a move.
 */
UCLASS()
class POKEMONBATTLE_API UBattleMoveFunctionCode : public UGameplayAbility {
    GENERATED_BODY()

public:
    UBattleMoveFunctionCode();

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Context)
    const TScriptInterface<IBattleMove>& GetMove() const;
    
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;
    bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, FGameplayTagContainer *OptionalRelevantTags) const override;
    void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

    void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

    
    
private:
    static TArray<AActor *> FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo &ActorInfo);
    
    void UseMove(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

protected:
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool MoveFailed(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets, const FRunningMessageSet& FailureMessages) const;

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Moves|Conclusion")
    void ProcessMoveFailure(const TArray<FText>& FailureMessages);

    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool WorksWithNoTargets();

    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool FailsAgainstTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet& FailureMessages) const;

    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Display")
    bool DisplayMessagesAndAnimation(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets, const TArray<FText>& Messages);

private:
    bool HitCheck(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet& FailureMessages);
    
    UPROPERTY(BlueprintGetter = GetMove, Category = Context)
    TScriptInterface<IBattleMove> BattleMove;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageEffect;

    bool bSucceededAgainstTarget = false;
    
    /**
     * The attribute that is used to determine the amount of PP a move should be reduced by
     */
    UPROPERTY()
    FGameplayAttribute CostAttribute = UPokemonCoreAttributeSet::GetMoveCostAttribute();

};

UCLASS()
class UBattleMoveFunctionCodeHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Moves|Messages")
    static TArray<FText>& GetMessages(const FRunningMessageSet& Messages);
    
};