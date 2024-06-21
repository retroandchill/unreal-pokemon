// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CriticalHitRateModificationTrait.h"
#include "CriticalHitOverrideTrait.generated.h"

/**
 * Applies a precedence-based change to the critical override modifier.
 */
UCLASS()
class POKEMONBATTLE_API UCriticalHitOverrideTrait : public UCriticalHitRateModificationTrait {
    GENERATED_BODY()

public:
    void Apply_Implementation(int32 &Stages, ECriticalOverride &Override, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const override;

private:
    /**
     * The override state of the critical hits to attempt to change it to
     */
    UPROPERTY(EditAnywhere, Category = Damage)
    ECriticalOverride OverrideValue;

};
