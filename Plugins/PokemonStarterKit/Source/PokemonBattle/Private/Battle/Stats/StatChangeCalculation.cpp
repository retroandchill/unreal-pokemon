// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Stats/StatChangeCalculation.h"
#include "Battle/Stats/StatTags.h"

float UStatChangeCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const {
    float Change = Spec.GetSetByCallerMagnitude(Pokemon::Battle::Stats::StagesTag);
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
    check(TargetTags != nullptr)

    if (TargetTags->HasTag(Pokemon::Battle::Stats::StatStagesInvertTag)) {
        Change *= -1.f;
    }
    
    return Change;
}