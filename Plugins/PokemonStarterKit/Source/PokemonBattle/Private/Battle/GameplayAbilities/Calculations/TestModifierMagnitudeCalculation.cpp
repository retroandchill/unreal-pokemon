// "Unreal Pokémon" created by Retro & Chill.


#include "TestModifierMagnitudeCalculation.h"
#include "Battle/GameplayAbilities/Attributes/DamageModificationAttributeSet.h"

UTestModifierMagnitudeCalculation::UTestModifierMagnitudeCalculation() {
    PowerMultiplierAttribute.AttributeToCapture = UDamageModificationAttributeSet::GetPowerMultiplierAttribute();
    PowerMultiplierAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    PowerMultiplierAttribute.bSnapshot = false;

    RelevantAttributesToCapture.Add(PowerMultiplierAttribute);
}

float UTestModifierMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const {
    float Multiplier;
    FAggregatorEvaluateParameters EvaluationParameters;
    GetCapturedAttributeMagnitude(PowerMultiplierAttribute, Spec, EvaluationParameters, Multiplier);

    static const auto GameplayTag = FGameplayTag::RequestGameplayTag("Test.Gameplay.Tag");
    float Magnitude = Spec.GetSetByCallerMagnitude(GameplayTag);
    
    return Multiplier * Magnitude;
}