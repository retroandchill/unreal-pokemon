// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/Moves/CriticalHitRateCalculationPayload.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "BattleMoveFunctionCode.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MoveEvaluationHelpers.generated.h"

class UAdditionalEffectChanceModificationPayload;
class UCriticalHitRateCalculationPayload;
/**
 * Helper library for move related evaluation.
 */
UCLASS()
class POKEMONBATTLE_API UMoveEvaluationHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Up the critical hit stages by the specified amount
     * @param Context The context for the move being used
     * @param Amount The amount to increment the stages by
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Critical Hits")
    static void IncrementCriticalHitRate(const UCriticalHitRateCalculationPayload *Context, int32 Amount = 1);

    /**
     * Up the critical hit stages by the specified amount if the user has the specified tag
     * @param Context The context for the move being used
     * @param Tag The tag that is required to perform the increment
     * @param Amount The amount to increment the stages by
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Critical Hits")
    static void IncrementCriticalHitRateIfUserHasTag(const UCriticalHitRateCalculationPayload *Context,
                                                     FGameplayTag Tag, int32 Amount = 1);

    /**
     * Take an existing critical override value and change it based on the new value and the existing rules for
     * precedence.
     * @param Old The old value
     * @param New The new value to attempt to assign to
     * @return The result after evaluation
     */
    UFUNCTION(BlueprintPure, Category = "Moves|Critical Hits")
    static ECriticalOverride ApplyCriticalHitOverride(ECriticalOverride Old, ECriticalOverride New);

    /**
     * Take an existing critical override value and change it based on the new value and the existing rules for
     * precedence.
     * @param Context The context object that contains the old value
     * @param Override The new value to attempt to assign to[]
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Critical Hits")
    static void SetCriticalHitOverride(const UCriticalHitRateCalculationPayload *Context, ECriticalOverride Override);

    /**
     * Is the user of the move of the specified species
     * @param Context The payload that contains all the move information
     * @param Species The species to verify
     * @return Is the user of the specified species
     */
    UFUNCTION(BlueprintPure, Category = "Moves|Context")
    static bool UserIsSpecies(const TScriptInterface<IMoveEventPayload> &Context, FName Species);

    /**
     * Boost the power of a move
     * @param Context The payload that contains all the move information
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostPower(const UDamageModificationPayload *Context, float Multiplier = 1.f);

    /**
     * Boost the power of a move if the user of the move has the specified gameplay tag.
     * @param Context The payload that contains all the move information
     * @param Tag The tag to search for
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostPowerIfUserHasTag(const UDamageModificationPayload *Context, FGameplayTag Tag,
                                       float Multiplier = 1.f);

    /**
     * Boost the power of a move if the user of the move has all of the specified gameplay tags.
     * @param Context The payload that contains all the move information
     * @param Tags The tags to search for
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostPowerIfUserHasAllTags(const UDamageModificationPayload *Context, FGameplayTagContainer Tags,
                                           float Multiplier = 1.f);

    /**
     * Boost the power of a move if the user of the move has any of the specified gameplay tags.
     * @param Context The payload that contains all the move information
     * @param Tags The tags to search for
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostPowerIfUserHasAnyTags(const UDamageModificationPayload *Context, FGameplayTagContainer Tags,
                                           float Multiplier = 1.f);

    /**
     * Boost the damage of a move if the user of the move has the specified gameplay tag.
     * @param Context The payload that contains all the move information
     * @param Multiplier The multiplier to apply if the tag is found
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Damage")
    static void BoostFinalDamageMultiplier(const UDamageModificationPayload *Context, float Multiplier = 1.f);

    /**
     * Multiply the additional effect chance by the supplied multiplier
     * @param Context The payload that contains the move information
     * @param Multiplier The multiplier to apply
     */
    UFUNCTION(BlueprintCallable, Category = "Moves|Effects")
    static void BoostAdditionalEffectChance(const UAdditionalEffectChanceModificationPayload *Context,
                                            float Multiplier = 1.f);
};