// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "MoveUsageAttributeSet.generated.h"

namespace Pokemon::Battle {
namespace CriticalHits {

constexpr float Normal = 0.f;
constexpr float Always = 1.f;
constexpr float Never = -1.f;

}
}

/**
 * Attributes that come into existence during move execution and disappear after the move has been resolved.
 */
UCLASS()
class POKEMONBATTLE_API UMoveUsageAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UMoveUsageAttributeSet, CriticalHitStages)
    ATTRIBUTE_ACCESSORS(UMoveUsageAttributeSet, CriticalHitRateOverride)
    
private:

    /**
     * The number of stages the critical hit rate has been raised by
     */
    UPROPERTY()
    FGameplayAttributeData CriticalHitStages = 0.f;
    
    /**
     * The factor to multiply the target's defense by
     */
    UPROPERTY()
    FGameplayAttributeData CriticalHitRateOverride = Pokemon::Battle::CriticalHits::Normal;
};
