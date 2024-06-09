// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMove.h"
#include "UObject/Object.h"
#include "BaseBattleMove.generated.h"

struct FDamageMultipliers;
struct FModifiedDamage;

/**
 * The base class used for all battle moves used by the game.
 */
UCLASS()
class POKEMONBATTLE_API UBaseBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

public:
    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IBattle>& Battle, const TScriptInterface<IMove> &Move) override;

protected:
    TScriptInterface<IBattle> GetOwningBattle_Implementation() const override;

public:
    bool IsConfusionAttack() const override;
    bool HasTag(FName Tag) const;
    
protected:
    FBattleDamage CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                 const TScriptInterface<IBattler> &Target,
                                                 int32 TargetCount) override;

    /**
     * Apply any modifiers related to type matchups
     * @param Effects The effects to the damage to apply
     * @param Target The target of the move
     * @param MoveType The type of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void CalculateTypeMatchups(UPARAM(Ref) FDamageEffects &Effects, const TScriptInterface<IBattler>& Target, FName MoveType);

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
    int32 CalculateBasePower(int32 MovePower, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target);

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

private:
    void CalculateDamageMultipliers(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount, FName Type, int32 BaseDamage, const FDamageEffects& Effects);
    void ApplyAbilityMultipliers(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, FName
                                 Type, int32 BaseDamage);
    void ApplyHoldItemMultipliers(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, FName
                                 Type, int32 BaseDamage);
    void ApplyUserEffects(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, FName
                                    Type, int32 BaseDamage);
    void ApplyTargetEffects(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, FName
                                    Type, int32 BaseDamage);
    void ApplyFieldEffects(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, FName
                                    Type, int32 BaseDamage);
    
protected:
    /**
    * Apply the modifer for a multi-target move
    * @param Multipliers The multipliers to apply the effect to
    * @param Effects The pre-computed damage effects
    */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyCriticalHitModifier(UPARAM(Ref) FDamageMultipliers& Multipliers, const FDamageEffects &Effects);
    
    /**
     * Apply the modifer for a multi-target move
     * @param Multipliers The multipliers to apply the effect to
     * @param TargetCount The number of targets
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyMultiTargetModifier(UPARAM(Ref) FDamageMultipliers& Multipliers, int32 TargetCount);
    
    /**
     * Apply the damage swing the damage at the end of the calculation
     * @param Multipliers The damage that has currently been calculated
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyDamageSwing(UPARAM(Ref) FDamageMultipliers &Multipliers);

    /**
     * Apply any modifiers related to Same-Type Attack Bonus (STAB)
     * @param Multipliers The damage multipliers to apply
     * @param User The user of the move
     * @param MoveType The type of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyStabModifiers(UPARAM(Ref) FDamageMultipliers &Multipliers, const TScriptInterface<IBattler>& User, FName MoveType);

    /**
     * Apply any modifiers related to Same-Type Attack Bonus (STAB)
     * @param Multipliers The damage multipliers to apply
     * @param Effects The pre-computed damage effects
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyTypeMatchUps(FDamageMultipliers &Multipliers, const FDamageEffects& Effects);

    /**
     * Apply any additional damage modifiers that need to be applied
     * @param Multipliers multipliers to apply
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The target count of the move
     * @param Type The type of the move
     * @param BaseDamage The base damage of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyAdditionalDamageModifiers(UPARAM(Ref) FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount, FName Type, int32 BaseDamage);

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