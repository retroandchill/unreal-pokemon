// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TypeMatchUpCalculation.generated.h"

/**
 * This serves as the base match-up calculation for types
 */
UCLASS()
class POKEMONBATTLE_API UTypeMatchUpCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

private:
    

};
