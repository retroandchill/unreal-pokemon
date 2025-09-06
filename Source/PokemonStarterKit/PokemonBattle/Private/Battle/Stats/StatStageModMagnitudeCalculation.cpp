// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Stats//StatStageModMagnitudeCalculation.h"
#include "AbilitySystemComponent.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "Battle/Stats/StatTags.h"
#include "Species/Stat.h"

float UStatStageModMagnitudeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const
{
    using enum EPokemonStatType;

    // Gather the tags from the source and target as that can affect which buffs should be used
    const FGameplayTagContainer *SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer *TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float FloatStages = 0.f;
    verify(GetCapturedAttributeMagnitude(StagesAttribute, Spec, EvaluationParameters, FloatStages))
    static auto &StageInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    int32 StagesLimit = StageInfo.Num();
    int32 Stages = FMath::Clamp(FMath::RoundToInt32(FloatStages), -StagesLimit, StagesLimit);

    // This effect gets added by the owner, so it's safe to assume the instigator component and the target component
    // are the same thing in this context.
    static auto &Lookup = Pokemon::Battle::Stats::FLookup::Get();
    auto InstigatorComponent = Spec.GetContext().GetInstigatorAbilitySystemComponent();
    if (Stages > 0 && !InstigatorComponent->HasMatchingGameplayTag(Lookup.GetIgnorePositiveTag(StatID)))
    {
        return StageInfo[Stages - 1].PositiveStatMultiplier;
    }

    if (Stages < 0 && !InstigatorComponent->HasMatchingGameplayTag(Lookup.GetIgnoreNegativeTag(StatID)))
    {
        return StageInfo[-Stages - 1].NegativeStatMultiplier;
    }

    return 1.f;
}

#if WITH_EDITOR
void UStatStageModMagnitudeCalculation::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    RelevantAttributesToCapture.Reset();
    RelevantAttributesToCapture.Emplace(StagesAttribute);
}
#endif