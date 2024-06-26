// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Calculations/DamageCalculation.h"
#include "DataManager.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/StatStagesAttributeSet.h"
#include "Species/Stat.h"

UDamageCalculation::UDamageCalculation() {
    using enum EGameplayEffectAttributeCaptureSource;
    AddAttributeCapture(Attack.Base, UPokemonCoreAttributeSet::GetAttackAttribute(), Source);
    AddAttributeCapture(Attack.Stages, UStatStagesAttributeSet::GetAttackStagesAttribute(), Source);
    AddAttributeCapture(Defense.Base, UPokemonCoreAttributeSet::GetDefenseAttribute(), Target);
    AddAttributeCapture(Defense.Stages, UStatStagesAttributeSet::GetDefenseStagesAttribute(), Target);
    AddAttributeCapture(SpecialAttack.Base, UPokemonCoreAttributeSet::GetSpecialAttackAttribute(), Source);
    AddAttributeCapture(SpecialAttack.Stages, UStatStagesAttributeSet::GetSpecialAttackStagesAttribute(), Source);
    AddAttributeCapture(SpecialDefense.Base, UPokemonCoreAttributeSet::GetSpecialDefenseAttribute(), Target);
    AddAttributeCapture(SpecialDefense.Stages, UStatStagesAttributeSet::GetSpecialDefenseStagesAttribute(), Target);
}

float UDamageCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const {
    return Super::CalculateBaseMagnitude_Implementation(Spec);
}

void UDamageCalculation::AddAttributeCapture(FGameplayEffectAttributeCaptureDefinition &CaptureDefinition,
                                             const FGameplayAttribute& Attribute, EGameplayEffectAttributeCaptureSource CaptureSource) {
    CaptureDefinition.AttributeToCapture = Attribute;
    CaptureDefinition.AttributeSource = CaptureSource;
    CaptureDefinition.bSnapshot = false;
    RelevantAttributesToCapture.Add(CaptureDefinition);
}