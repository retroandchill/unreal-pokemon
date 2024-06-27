// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "AccuracyModifiersAttributeSet.generated.h"

/**
 * Attribute set that is used for the modification of accuracy during move evaluation.
 */
UCLASS()
class POKEMONBATTLE_API UAccuracyModifiersAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UAccuracyModifiersAttributeSet, BaseAccuracy)
    ATTRIBUTE_ACCESSORS(UAccuracyModifiersAttributeSet, AccuracyStage)
    ATTRIBUTE_ACCESSORS(UAccuracyModifiersAttributeSet, AccuracyMultiplier)
    
private:
    UPROPERTY()
    FGameplayAttributeData BaseAccuracy;
    
    UPROPERTY()
    FGameplayAttributeData AccuracyStage;

    UPROPERTY()
    FGameplayAttributeData AccuracyMultiplier;
    
    UPROPERTY()
    FGameplayAttributeData EvasionStage;

    UPROPERTY()
    FGameplayAttributeData EvasionMultiplier;

};
