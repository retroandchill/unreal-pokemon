// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMoveFunctionCode.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MoveEvaluationHelpers.generated.h"

class UCriticalHitRateCalculationPayload;
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

    /**
     * Take an existing critical override value and change it based on the new value and the existing rules for precedence.
     * @param Context The context object that contains the old value
     * @param Override The new value to attempt to assign to[]
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Critical Hits")
    static void SetCriticalHitOverride(const UCriticalHitRateCalculationPayload* Context, ECriticalOverride Override);

    /**
     * Boost the power of a move if the user of the move has the specified gameplay tag.
     * @param Context The payload that contains all the move information
     * @param Tag The tag to search for
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostPowerIfUserHasTag(const UDamageModificationPayload* Context, FGameplayTag Tag, float Multiplier = 1.f);
};
