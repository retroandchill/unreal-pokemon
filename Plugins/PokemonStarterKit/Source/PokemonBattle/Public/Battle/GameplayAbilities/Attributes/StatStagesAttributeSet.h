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
class POKEMONBATTLE_API UStatStagesAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, AttackStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, DefenseStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpecialAttackStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpecialDefenseStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, SpeedStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, AccuracyStages)
    ATTRIBUTE_ACCESSORS(UStatStagesAttributeSet, EvasionStages)

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

};
