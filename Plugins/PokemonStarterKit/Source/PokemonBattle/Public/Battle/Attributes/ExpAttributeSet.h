// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"

#include "ExpAttributeSet.generated.h"

/**
 * Attributes related to Exp. and EV gain.
 */
UCLASS()
class POKEMONBATTLE_API UExpAttributeSet : public UAttributeSet {
    GENERATED_BODY()

  public:
    ATTRIBUTE_ACCESSORS(UExpAttributeSet, ExpGainRate)

  private:
    UPROPERTY()
    FGameplayAttributeData ExpGainRate = 1.f;
};
