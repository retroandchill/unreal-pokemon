// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"

#include "TargetDamageStateAttributeSet.generated.h"

/**
 * Attribute set for the damage state of a given target.
 */
UCLASS()
class POKEMONBATTLE_API UTargetDamageStateAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

  public:
    ATTRIBUTE_ACCESSORS(UTargetDamageStateAttributeSet, TypeMod)
    ATTRIBUTE_ACCESSORS(UTargetDamageStateAttributeSet, CalculatedDamage)

    void Reset();

  private:
    UPROPERTY()
    FGameplayAttributeData TypeMod;

    UPROPERTY()
    FGameplayAttributeData CalculatedDamage;
};
