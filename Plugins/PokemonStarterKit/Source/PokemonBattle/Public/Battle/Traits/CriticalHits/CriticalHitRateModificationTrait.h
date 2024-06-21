// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleDamage.h"
#include "UObject/Object.h"
#include "CriticalHitRateModificationTrait.generated.h"

class IBattler;
class UCriticalHitRateModificationCondition;

/**
 * A simple trait for modifying a move's critical hit rate
 */
UCLASS(Abstract, EditInlineNew)
class POKEMONBATTLE_API UCriticalHitRateModificationTrait : public UObject {
    GENERATED_BODY()

public:
    /**
     * Does the trait meet the conditions
     * @param User The user of the move
     * @param Target The target of the move
     * @return Will the trait be applied.
     */
    bool MeetsConditions(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) const;

    /**
     * Apply the trait to the existing multipliers
     * @param Stages
     * @param Override
     * @param User The user of the move
     * @param Target The target of the move
     * @return The amount to add or subtract from the rate
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void Apply(UPARAM(Ref) int32 &Stages, UPARAM(Ref) ECriticalOverride& Override, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const;

private:
    /**
     * Any conditions that must be met to apply this trait. Having no conditions means the trait is always active.
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Damage)
    TArray<TObjectPtr<UCriticalHitRateModificationCondition>> Conditions;

};
