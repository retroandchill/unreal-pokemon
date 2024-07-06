// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Events/BattleMessage.h"

#include "BattleMoveFunctionCode.generated.h"

class UDamageModificationPayload;
class AMoveAnimation;
class AMoveExecutor;
class IBattler;
class IBattleMove;
class IMove;

/**
 * The override state for a critical hit.
 */
UENUM(BlueprintType)
enum class ECriticalOverride : uint8 {
    /**
     * Roll a critical hit normally.
     */
    Normal,

    /**
     * The move always rolls a critical hit
     */
    Always,

    /**
     * The move can never land a crit.
*/
    Never
    
};

/**
 * Information about a captured stat
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FCapturedBattleStat {
    GENERATED_BODY()
    
    /**
     * The battler that owns the stat in question
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moves|Damage");
    TScriptInterface<IBattler> OwningBattler;

    /**
     * The attribute that underpins the stat
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moves|Damage")
    FName StatID;

    /**
     * Get the value of the stat for the battler
     * @return The value of the stat
     */
    int32 GetStatValue() const;
    
};

/**
 * The structure for the attack and defense stats used in damage calculation
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FAttackAndDefenseStats {
    GENERATED_BODY()

    /**
     * The attack stat to use (generally from the user)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Moves|Damage")
    FCapturedBattleStat Attack;

    /**
     * The defense stat to use (generally from the target)
     */
    UPROPERTY(BlueprintReadOnly, Category = "Moves|Damage")
    FCapturedBattleStat Defense;
};

/**
 * The gameplay ability for using a move.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API UBattleMoveFunctionCode : public UGameplayAbility {
    GENERATED_BODY()

public:
    /**
     * Create the CDO for this class
     */
    UBattleMoveFunctionCode();

    /**
     * Get the underlying move that this class wraps around.
     * @return The underlying move that this class wraps around.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Context)
    const TScriptInterface<IBattleMove>& GetMove() const;

    bool ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayEventData *Payload) const override;
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;
    void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    /**
     * Get the running messages for this ability
     * @return The running messages of this ability
     */
    const FRunningMessageSet &GetRunningMessage() const;
protected:
    /**
     * Determine the type of the move.
     * @return The calculated type
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Types")
    FName DetermineType() const;
    
private:
    /**
     * Filter out any invalid targets from this move
     * @param Handle The handle for this ability
     * @param ActorInfo The information about the owning actor
     * @param TriggerEventData
     * @return The array of filtered target actors
     */
    static TArray<AActor *> FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo &ActorInfo, const FGameplayEventData* TriggerEventData);

    /**
     * Use the move on the given targets
     * @param User The user of the move
     * @param Targets The targets of the move
     */
    void UseMove(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

protected:
    /**
     * Check if the move failed completely
     * @param User The user of the move
     * @param Targets The targets of the move
     * @param FailureMessages The handle for any failure messages to display
     * @return Did the move fail?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool MoveFailed(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets, const FRunningMessageSet& FailureMessages) const;

    /**
     * Process that happens when a move fails.
     * @param FailureMessages The messages display to the player
     */
    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Moves|Conclusion")
    void ProcessMoveFailure(const FRunningMessageSet& FailureMessages);

    /**
     * Can this move be used without any targets?
     * @return Can this move be used without any targets?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool WorksWithNoTargets();

    /**
     * Perform a success check against a single target
     * @param User The user of the move
     * @param Target The target to evaluate against
     * @param FailureMessages The messages display to the player
     * @return Did the move succeed against the target
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool SuccessCheckAgainstTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet &FailureMessages);

    /**
     * Check if this move fails against a target
     * @param User The user of the move
     * @param Target The target to evaluate against
     * @param FailureMessages The messages display to the player
     * @return Did the move fail?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool FailsAgainstTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet& FailureMessages) const;

    /**
     * Perform a hit check against the target
     * @param User The user of the move
     * @param Target The target to evaluate against
     * @return Did the move hit?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    bool HitCheck(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Calculate the base accuracy of the move
     * @param Accuracy The stated accuracy of the move
     * @param User The user of the move
     * @param Target The target to evaluate against
     * @return The base accuracy to use in the hit check
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Success Checking")
    int32 CalculateBaseAccuracy(int32 Accuracy, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Display the given messages and play the animation of the move
     * @param User The user of the move
     * @param Targets The target of the move in question
     * @param Messages The messages to be displayed
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Display")
    void DisplayMessagesAndAnimation(const TScriptInterface<IBattler> &User,
                                     const TArray<TScriptInterface<IBattler>> &Targets, const FRunningMessageSet& Messages);

    /**
     * Take the damage effects of the move and apply them to the target
     * @param User The user of the move
     * @param Targets The targets to deal damage to
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    void DealDamage(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

    /**
     * Calculate the damage dealt to a single target
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The total number of target's being hit
     * @param PreDamageMessages The messages to display before damage is dealt
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    void CalculateDamageAgainstTarget(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                      int32 TargetCount, const FRunningMessageSet &PreDamageMessages);

    /**
     * Apply any move-specifc damage multipliers
     * @param Payload The event payload that can be further modified
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Damage")
    void ApplyAdditionalDamageMultipliers(UDamageModificationPayload* Payload);

    /**
     * Calculate the base power of the move
     * @param Power The stated power of the move
     * @param User The user of the move
     * @param Target The target of the move in question
     * @return The base power to use in damage calculation
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    int32 CalculateBasePower(int32 Power, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Get the attack and defense stat of the use
     * @param User The user of the move
     * @param Target The target of the move
     * @return The attack and defense stat to use in calculation
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    FAttackAndDefenseStats GetAttackAndDefense(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Check if the move scored a critical hit
     * @param User The user of the move
     * @param Target The target of the move
     * @return Did the move score a crit?
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    bool IsCritical(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Get the critical override state of the move.
     * @param User The user of the move
     * @param Target The target of the move
     * @return The new critical override state
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    ECriticalOverride GetCriticalOverride(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Calculate the type match-up against a target
     * @param MoveType The type of the move for calculation
     * @param User The user of the move
     * @param Target The target of the move
     * @return The multiplier for the matchup
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    float CalculateTypeMatchUp(FName MoveType, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Calculate the type modifier for a single type
     * @param AttackingType The attacking type
     * @param DefendingType The defending type
     * @param User The user of the move
     * @param Target The target of the move
     * @return The multiplier for the given type
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Damage")
    float CalculateSingleTypeMod(FName AttackingType, FName DefendingType, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Display any damage to the user from the 
     * @param User The user of the move
     * @param Targets The targets of the move in question
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Damage")
    void DisplayDamage(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

    /**
     * Apply any move effects to the targets
     * @param User The user of the move
     * @param Targets The targets of the move
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Effects")
    void ApplyMoveEffects(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);

    /**
     * Effect applied to a target that took damage from a move
     * @param User The user of the move
     * @param Target the target of the move
     * @param Messages The running set of messages to be displayed after the effects are applied
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Effects")
    void ApplyEffectWhenDealingDamage(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet& Messages);
    
    /**
     * Apply any guaranteed effects against a target
     * @param User The user of the move
     * @param Target The target of the move
     * @param Messages The running set of messages to be displayed after the effects are applied
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Effects")
    void ApplyEffectAgainstTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet& Messages);

    /**
     * Apply any generate effects that don't depend on any targets
     * @param User The user of the move
     * @param Messages The running set of messages to be displayed after the effects are applied
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Effects")
    void ApplyGeneralEffect(const TScriptInterface<IBattler>& User, const FRunningMessageSet& Messages);

    /**
     * Perform a faint check on the user and targets before applying the additional effects
     * @param User The user of the move
     * @param Targets the targets of the move
     * @param Messages The running set of messages to be displayed after the effects are applied
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Effects")
    void FaintCheck(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets, const FRunningMessageSet
                    & Messages);

    /**
     * Apply additional effects against the user
     * @param User The user of the move
     * @param Targets The targets of the move
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Effects")
    void ApplyAdditionalEffects(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets);
    
    /**
     * Apply any additional effect to a target
     * @param User The user of the move
     * @param Target The target of the move
     * @param Messages
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Effects")
    void ApplyAdditionalEffect(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, const FRunningMessageSet&
                               Messages);

    /**
     * Calculate the value of a move's additional effect chance
     * @param User The user of the move in question
     * @param Target The target of the move
     * @return The chance of the additional effect occurring
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Moves|Effects")
    int32 CalculateAdditionalEffectChance(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Display the move effects to the player and end the move
     * @param User The user of the move
     * @param Targets The targets of the move
     * @param Messages
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Conclusion")
    void DisplayMoveEffectsAndEndMove(const TScriptInterface<IBattler>& User, const TArray<TScriptInterface<IBattler>>& Targets, const FRunningMessageSet
                                      & Messages);

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = GameplayEffects)
    TArray<FActiveGameplayEffectHandle> ApplyGameplayEffectToBattler(const TScriptInterface<IBattler> &Battler, TSubclassOf<UGameplayEffect> EffectClass, int32 Level, int32 Stacks) const;
    
private:
    /**
     * The underlying move that this ability wraps
     */
    UPROPERTY(BlueprintGetter = GetMove, Category = Context)
    TScriptInterface<IBattleMove> BattleMove;

    /**
     * Any gameplay tags that were added to the user
     */
    FGameplayTagContainer AddedTags;

    /**
     * The added tags to the targets
     */
    UPROPERTY()
    TMap<TObjectPtr<AActor>, FGameplayTagContainer> AddedTargetTags;

    /**
     * Type that was determined for the move
     */
    FName DeterminedType;

    /**
     * The effect used to deal damage to the opposing Pokémon
     */
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffects)
    TSubclassOf<UGameplayEffect> DealDamageEffect;
    
    /**
     * The attribute that is used to determine the amount of PP a move should be reduced by
     */
    UPROPERTY()
    FGameplayAttribute CostAttribute = UPokemonCoreAttributeSet::GetMoveCostAttribute();

    /**
     * Get the running messages for the move's execution. This is periodically cleared between the Blueprint events used
     * to print them.
     */
    UPROPERTY()
    FRunningMessageSet RunningMessages;
};