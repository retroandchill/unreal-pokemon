// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModificationCondition.h"
#include "MoveTypeMatches.generated.h"

/**
 * Condition for when a move's type matches a specific one
 */
UCLASS()
class POKEMONBATTLE_API UMoveTypeMatches : public UDamageModificationCondition {
    GENERATED_BODY()

public:
    bool Evaluate_Implementation(const FMoveDamageInfo &Context) const override;

private:
    /**
     * The type that needs to match the move's type
     */
    UPROPERTY(EditAnywhere, Category = "Criteria", meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    FName Type;
};
