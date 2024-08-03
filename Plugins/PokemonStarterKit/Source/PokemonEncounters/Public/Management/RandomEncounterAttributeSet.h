// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTags/AttributeMacros.h"

#include "RandomEncounterAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONENCOUNTERS_API URandomEncounterAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(URandomEncounterAttributeSet, EncounterStepModifier)

private:
    /**
     * The modifier applied to random encounter calculations when walking. Mainly used to reduce the encounter rate
     * by 20% when on the bicyle.
     */
    UPROPERTY()
    FGameplayAttributeData EncounterStepModifier = 1.f;
    
};
