// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModificationTrait.h"

#include "ChooseOneDamageModification.generated.h"

/**
 * Composite damage modification trait that chooses the first one that satisfies conditions.
 */
UCLASS()
class POKEMONBATTLE_API UChooseOneDamageModification : public UDamageModificationTrait {
    GENERATED_BODY()

  protected:
    void Apply_Implementation(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) const override;

  private:
    /**
     * The list of traits to consider before applying one
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> Traits;
};
