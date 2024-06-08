// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMove.h"
#include "Modifiers/TargetCountModifier.h"
#include "UObject/Object.h"
#include "BaseBattleMove.generated.h"

struct FModifiedDamage;

namespace Battle::Moves {
    class IDamageModifier;
}

/**
 * The base class used for all battle moves used by the game.
 */
UCLASS()
class POKEMONBATTLE_API UBaseBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

public:
    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IMove> &Move) override;

protected:
    FBattleDamage CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                 const TScriptInterface<IBattler> &Target,
                                                 int32 TargetCount) const override;

public:
    /**
     * Determine the type of the move to be used
     * @return The particular move type to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FName DetermineType() const;

    /**
     * Calculate the base damage of the particular move before any modifiers are applied
     * @param User The user of the move
     * @param Target The target of the move
     * @return The base damage, pre-modification
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateBaseDamage(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

    /**
     * Calculate the true power of the move, pulled from the move's base damage
     * @param User The user of the move
     * @param Target The target of the move
     * @return The actual power value to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateTruePower(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

    /**
     * Fetch the attack and defense stats to use for damage
     * @param User The user of the move
     * @param Target The target of the move
     * @return The values of the stats to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FAttackAndDefense GetAttackAndDefense(const TScriptInterface<IBattler> &User,
                                          const TScriptInterface<IBattler> &Target) const;

    /**
     * Apply any additional damage modifiers that need to be applied
     * @param Damage The currently calculated damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The target count of the move
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyAdditionalDamageModifiers(UPARAM(Ref) FModifiedDamage &Damage, const TScriptInterface<IBattler> &User,
                                        const TScriptInterface<IBattler> &Target, int32 TargetCount) const;

protected:
    /**
     * Apply the damage swing the damage at the end of the calculation
     * @param Damage The damage that has currently been calculated
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ApplyDamageSwing(UPARAM(Ref) FModifiedDamage &Damage) const;

private:
    /**
     * The move that is actively being wrapped around
     */
    UPROPERTY()
    TScriptInterface<IMove> WrappedMove;

    /**
     * The array of low-level damage modifiers that are innate to the move effect
     */
    TArray<TUniquePtr<Battle::Moves::IDamageModifier>> DamageModifiers;
};