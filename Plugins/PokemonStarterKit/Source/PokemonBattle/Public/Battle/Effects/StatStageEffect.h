// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StatStageEffect.generated.h"

/**
 * Specialized effect for handling the information regarding stat changes.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API UStatStageEffect : public UGameplayEffect {
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    /**
     * The curve table row used to get the multipliers for each stage for all stats except accuracy and evasion
     */
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle MainStatMultipliers;

    /**
     * The curve table row used to get the divisors for each stage for all stats except accuracy and evasion
     */
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle MainStatDivisors;

    /**
     * The curve table row used to get the multipliers for each stage for accuracy and evasion
     */
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle BattleStatMultipliers;

    /**
     * The curve table row used to get the divisors for each stage for accuracy and evasion
     */
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle BattleStatDivisors;

};
