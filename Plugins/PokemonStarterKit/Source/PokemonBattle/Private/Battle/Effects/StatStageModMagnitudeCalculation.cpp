// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/StatStageModMagnitudeCalculation.h"
#include "Settings/BaseSettings.h"

float UStatStageModMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const {
    using enum EPokemonStatType;
    
    // Gather the tags from the source and target as that can affect which buffs should be used
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float FloatStages = 0.f;
    verify(GetCapturedAttributeMagnitude(StagesAttribute, Spec, EvaluationParameters, FloatStages))
    static auto &StageInfo = Pokemon::FBaseSettings::Get().GetStatStages();
    int32 StagesLimit = StageInfo.Num();
    int32 Stages = FMath::Clamp(FMath::RoundToInt32(FloatStages), -StagesLimit, StagesLimit);
    if (Stages > 0) {
        return StageInfo[Stages - 1].PositiveStatMultiplier;
    }

    if (Stages < 0) {
        return StageInfo[-Stages - 1].NegativeStatMultiplier;
    }

    return 1.f;
}


#if WITH_EDITOR
void UStatStageModMagnitudeCalculation::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    RelevantAttributesToCapture.Reset();
    RelevantAttributesToCapture.Emplace(StagesAttribute);
}
#endif