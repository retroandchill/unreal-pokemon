// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"

#include "RandomEncounterAttributeSet.generated.h"

/**
 *
 */
UCLASS()
class POKEMONENCOUNTERS_API URandomEncounterAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

  public:
    ATTRIBUTE_ACCESSORS(URandomEncounterAttributeSet, EncounterStepModifier)
    ATTRIBUTE_ACCESSORS(URandomEncounterAttributeSet, EncounterChanceModifier)
    ATTRIBUTE_ACCESSORS(URandomEncounterAttributeSet, MinStepsNeededModifier)

  private:
    /**
     * The modifier applied to random encounter calculations when walking. Mainly used to reduce the encounter rate
     * by 20% when on the bicyle.
     */
    UPROPERTY()
    FGameplayAttributeData EncounterStepModifier = 1.f;

    /**
     * The direct modifier to the encounter chance
     */
    UPROPERTY()
    FGameplayAttributeData EncounterChanceModifier = 1.f;

    /**
     * The direct modifier to the minimum number of steps before regular encounter rolls resume
     */
    UPROPERTY()
    FGameplayAttributeData MinStepsNeededModifier = 1.f;
};
