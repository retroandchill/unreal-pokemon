// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "StatChangeCalculation.generated.h"

/**
 * The calculation class for applying a stat change to
 */
UCLASS(Blueprintable)
class POKEMONBATTLE_API UStatChangeCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;
};
