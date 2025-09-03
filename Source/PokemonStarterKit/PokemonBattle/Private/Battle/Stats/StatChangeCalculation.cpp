// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Stats/StatChangeCalculation.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Stats/StatTags.h"

UStatChangeCalculation::UStatChangeCalculation() {
    StatChangeRateAttribute.AttributeToCapture = UStatStagesAttributeSet::GetStatStageMultiplierAttribute();
    StatChangeRateAttribute.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    StatChangeRateAttribute.bSnapshot = false;
    RelevantAttributesToCapture.Emplace(StatChangeRateAttribute);
}

float UStatChangeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const {
    // Gather the tags from the source and target as that can affect which buffs should be used
    const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Multiplier;
    verify(GetCapturedAttributeMagnitude(StatChangeRateAttribute, Spec, EvaluationParameters, Multiplier))
    float Change = Spec.GetSetByCallerMagnitude(Pokemon::Battle::Stats::StagesTag);
    return Change * Multiplier;
}