// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "StabModCalculation.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UStabModCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

};
