// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/Damage/DamageModificationTrait.h"

#include "SimpleDamageModificationTrait.generated.h"

/**
 * A damage modification trait that applies a simple multiplier with no additional modifications
 */
UCLASS()
class POKEMONBATTLE_API USimpleDamageModificationTrait : public UDamageModificationTrait {
    GENERATED_BODY()

  protected:
    void Apply_Implementation(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) const override;

  private:
    /**
     * The multipliers to apply when calculating damage
     */
    UPROPERTY(EditAnywhere, Category = Damage)
    FDamageMultipliers DamageMultipliers;
};
