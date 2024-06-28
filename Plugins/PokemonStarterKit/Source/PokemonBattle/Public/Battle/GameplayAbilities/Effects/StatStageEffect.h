// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "StatStageEffect.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class POKEMONBATTLE_API UStatStageEffect : public UGameplayEffect {
    GENERATED_BODY()

public:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle MainStatMultipliers;

    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle MainStatDivisors;

    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle BattleStatMultipliers;

    UPROPERTY(EditDefaultsOnly, Category = GameplayEffect)
    FCurveTableRowHandle BattleStatDivisors;

};
