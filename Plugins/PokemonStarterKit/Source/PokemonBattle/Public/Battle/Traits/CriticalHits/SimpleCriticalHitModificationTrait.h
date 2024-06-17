// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CriticalHitRateModificationTrait.h"
#include "SimpleCriticalHitModificationTrait.generated.h"

/**
 * A simple critical hit modification trait that increases/decreases the stages by a given amount.
 */
UCLASS()
class POKEMONBATTLE_API USimpleCriticalHitModificationTrait : public UCriticalHitRateModificationTrait {
    GENERATED_BODY()

protected:
    void Apply_Implementation(int32 &Value, ECriticalOverride& Override, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const override;

private:
    UPROPERTY(EditAnywhere, Category = Damage)
    int32 Change;

};
