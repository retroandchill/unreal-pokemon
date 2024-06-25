// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMove.h"
#include "Abilities/GameplayAbility.h"

#include "BaseBattleMove.generated.h"

class UMoveEffectContext;
struct FDamageMultipliers;
struct FModifiedDamage;

/**
 * The base class used for all battle moves used by the game.
 */
UCLASS(Blueprintable)
class POKEMONBATTLE_API UBaseBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

  public:
    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattle> &Battle,
                                             const TScriptInterface<IMove> &Move) override;

  protected:
    bool IsUsable_Implementation() const override;
    TArray<TScriptInterface<IBattler>>
    GetAllPossibleTargets_Implementation(const TScriptInterface<IBattler> &User) const override;
    FText GetDisplayName_Implementation() const override;
    int32 GetCurrentPP_Implementation() const override;
    int32 GetMaxPP_Implementation() const override;
    FName GetDisplayType_Implementation() const override;
    int32 GetPriority_Implementation() const override;
    void PayCost_Implementation() override;
    TScriptInterface<IBattle> GetOwningBattle_Implementation() const override;

  public:
    bool IsConfusionAttack() const override;
    bool HasHighCriticalHitRate() const override;
    bool HasTag(FName Tag) const override;
    
  protected:
    bool PerformHitCheck_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) override;
    FBattleDamage CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                 const TScriptInterface<IBattler> &Target, int32 TargetCount) override;

private:
    /**
     * Roll for a critical hit
     * @param User The user of the move
     * @param Target The target for the move
     * @return Does the move result in a critical hit
     */
    bool IsCritical(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

protected:
    /**
     * Determine the critical override state of the move
     * @param User The user of the move
     * @param Target The target for the move
     * @return What is the critical override state
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Damage")
    ECriticalOverride GetCriticalOverride(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

    /**
     * Apply any modifiers related to type matchups
     * @param Effects The effects to the damage to apply
     * @param Target The target of the move
     * @param MoveType The type of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void CalculateTypeMatchups(UPARAM(Ref) FDamageEffects &Effects, const TScriptInterface<IBattler> &Target,
                               FName MoveType);

    /**
     * Determine the type of the move to be used
     * @return The particular move type to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FName DetermineType();

    /**
     * Calculate the base power of the move applying any necessary modifications
     * @param MovePower The base power of the move pre-modification
     * @param User The user of the move
     * @param Target The target for the move
     * @return
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateBasePower(int32 MovePower, const TScriptInterface<IBattler> &User,
                             const TScriptInterface<IBattler> &Target);

    /**
     * Calculate the base accuracy of the move in question
     * @param Accuracy The stated accuracy of the move
     * @param User The user of the move in question
     * @param Target The target of the move
     * @return The calculated move accuracy
     */
    UFUNCTION(BlueprintNativeEvent, Category = "Success Checking")
    int32 CalculateBaseAccuracy(int32 Accuracy, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

    /**
     * Calculate the base damage of the particular move before any modifiers are applied
     * @param Power The move's power
     * @param Level The user's level
     * @param Attack The attack stat to use
     * @param Defense The defense stat to use
     * @return The base damage
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateBaseDamage(int32 Power, int32 Level, int32 Attack, int32 Defense);

    /**
     * Fetch the attack and defense stats to use for damage
     * @param User The user of the move
     * @param Target The target of the move
     * @return The values of the stats to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FAttackAndDefense GetAttackAndDefense(const TScriptInterface<IBattler> &User,
                                          const TScriptInterface<IBattler> &Target);
    
    /**
     * Apply any additional damage modifiers that need to be applied
     * @param Multipliers multipliers to apply
     * @param Context The context in which this move is being used
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Damage)
    void ApplyAdditionalDamageModifiers(const FDamageMultiplierHandler &Multipliers, const FMoveDamageInfo &Context);

  private:
    /**
     * The current battle that is on-going
     */
    UPROPERTY()
    TScriptInterface<IBattle> CurrentBattle;

    /**
     * The move that is actively being wrapped around
     */
    UPROPERTY()
    TScriptInterface<IMove> WrappedMove;
};