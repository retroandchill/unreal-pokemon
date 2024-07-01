// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMoveFunctionCode.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoveEvaluationHelpers.generated.h"

/**
 * Helper library for move related evaluation.
 */
UCLASS()
class POKEMONBATTLE_API UMoveEvaluationHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Take an existing critical override value and change it based on the new value and the existing rules for precedence.
     * @param Old The old value
     * @param New The new value to attempt to assign to
     * @return The result after evaluation
     */
    UFUNCTION(BlueprintPure, Category = "Moves|Critical Hits")
    static ECriticalOverride ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New);
};
