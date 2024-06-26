// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "DamageCalculation.generated.h"

USTRUCT()
struct POKEMONBATTLE_API FStatAttributeData {
    GENERATED_BODY()

    UPROPERTY()
    FGameplayEffectAttributeCaptureDefinition Base;

    UPROPERTY()
    FGameplayEffectAttributeCaptureDefinition Stages;
};

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UDamageCalculation : public UGameplayModMagnitudeCalculation {
    GENERATED_BODY()

public:
    UDamageCalculation();

    float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

private:
    void AddAttributeCapture(FGameplayEffectAttributeCaptureDefinition& CaptureDefinition, const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource CaptureSource);
    
    UPROPERTY()
    FStatAttributeData Attack;
    
    UPROPERTY()
    FStatAttributeData Defense;
    
    UPROPERTY()
    FStatAttributeData SpecialAttack;
    
    UPROPERTY()
    FStatAttributeData SpecialDefense;
};
