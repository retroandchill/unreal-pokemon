// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModificationCondition.h"
#include "GlobalTraitTagExists.generated.h"

/**
 * Check for a trait tag existing on any trait holder on the field.
 */
UCLASS()
class POKEMONBATTLE_API UGlobalTraitTagExists : public UDamageModificationCondition {
    GENERATED_BODY()

protected:
    bool Evaluate_Implementation(const FMoveDamageInfo &Context) const override;

private:
    /**
     * The tag to match
     */
    UPROPERTY(EditAnywhere, Category = Criteria)
    FName Tag;
};
