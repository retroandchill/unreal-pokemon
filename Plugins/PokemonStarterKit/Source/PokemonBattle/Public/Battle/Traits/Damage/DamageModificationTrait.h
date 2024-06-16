// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleDamage.h"
#include "UObject/Object.h"
#include "DamageModificationTrait.generated.h"


class UDamageModificationCondition;

/**
 * Definition of a trait to modify damage in some manner.
 */
UCLASS(Abstract, EditInlineNew)
class POKEMONBATTLE_API UDamageModificationTrait : public UObject {
    GENERATED_BODY()

public:
    /**
     * Does the trait meet the conditions
     * @param Context The context of the move being used
     * @return Will the trait be applied.
     */
    bool MeetsConditions(const FMoveDamageInfo& Context) const;

    /**
     * Apply the trait to the existing multipliers
     * @param Multipliers The multipliers we're applying
     * @param Context The context of the move being used
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void Apply(FDamageMultipliers& Multipliers, const FMoveDamageInfo& Context) const;
    
private:
    /**
     * Any conditions that must be met to apply this trait. Having no conditions means the trait is always active.
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationCondition>> Conditions;

};
