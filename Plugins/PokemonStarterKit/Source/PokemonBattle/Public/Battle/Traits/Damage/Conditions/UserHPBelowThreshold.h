// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModificationCondition.h"
#include "UserHPBelowThreshold.generated.h"

/**
 * The user's HP must be below a certain percentage threshold.
 */
UCLASS(DisplayName = "User HP Below Threshold")
class POKEMONBATTLE_API UUserHPBelowThreshold : public UDamageModificationCondition {
    GENERATED_BODY()

public:
    bool Evaluate_Implementation(const FMoveDamageInfo &Context) const override;

private:
    /**
     * The HP threshold (as a range between 0 and 1)
     */
    UPROPERTY(EditAnywhere, Category = "Criteria", meta = (UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1))
    float Threshold;

};
