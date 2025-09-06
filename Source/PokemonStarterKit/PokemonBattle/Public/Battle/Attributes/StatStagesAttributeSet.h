// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"

#include "StatStagesAttributeSet.generated.h"

/**
 * Attribute set for handling stat stages
 */
UCLASS()
class POKEMONBATTLE_API UStatStagesAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

  public:
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, AttackStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, DefenseStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpecialAttackStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpecialDefenseStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpeedStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, AccuracyStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, EvasionStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, CriticalHitStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SameTypeAttackBonus)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, StatStageMultiplier)

    void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;

  private:
    UPROPERTY()
    FGameplayAttributeData AttackStages;

    UPROPERTY()
    FGameplayAttributeData DefenseStages;

    UPROPERTY()
    FGameplayAttributeData SpecialAttackStages;

    UPROPERTY()
    FGameplayAttributeData SpecialDefenseStages;

    UPROPERTY()
    FGameplayAttributeData SpeedStages;

    UPROPERTY()
    FGameplayAttributeData AccuracyStages;

    UPROPERTY()
    FGameplayAttributeData EvasionStages;

    /**
     * The number of stages the critical hit rate has been raised by
     */
    UPROPERTY()
    FGameplayAttributeData CriticalHitStages = 0.f;

    /**
     * The property that is multiplied to apply the Same-Type Attack Bonus (STAB). This will be 1, when the move
     * type does not match.
     */
    UPROPERTY()
    FGameplayAttributeData SameTypeAttackBonus = 1.f;

    /**
     * The amount to multiply any state stage changes by
     */
    UPROPERTY()
    FGameplayAttributeData StatStageMultiplier = 1.f;
};
