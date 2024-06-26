// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AttributeMacros.h"
#include "AttributeSet.h"
#include "Battle/Moves/BattleDamage.h"
#include "Settings/BaseSettings.h"

#include "CriticalHitRateModificationAttributeSet.generated.h"

namespace CriticalHits {

constexpr float Normal = 0.f;
constexpr float Always = 1.f;
constexpr float Never = -1.f;

}

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UCriticalHitRateModificationAttributeSet : public UAttributeSet {
    GENERATED_BODY()

public:
    ATTRIBUTE_ACCESSORS(UCriticalHitRateModificationAttributeSet, CriticalHitStages)
    ATTRIBUTE_ACCESSORS(UCriticalHitRateModificationAttributeSet, CriticalHitRateOverride)

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
    FGameplayAttributeData CriticalHitRateOverride = CriticalHits::Normal;
};
