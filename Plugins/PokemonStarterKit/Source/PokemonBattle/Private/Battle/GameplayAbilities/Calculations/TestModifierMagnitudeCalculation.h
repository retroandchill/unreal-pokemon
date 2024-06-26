// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "TestModifierMagnitudeCalculation.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UTestModifierMagnitudeCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    UTestModifierMagnitudeCalculation();
    
    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

private:
    FGameplayEffectAttributeCaptureDefinition PowerMultiplierAttribute;
};
