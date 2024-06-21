// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/CriticalHits/Conditions/CriticalHitRateModificationCondition.h"
#include "TestCritRateModCondition.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONTESTS_API UTestCritRateModCondition : public UCriticalHitRateModificationCondition {
    GENERATED_BODY()

public:
    bool bState = false;

    bool Evaluate_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const override;

};
