﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Moves/MoveDamageCategory.h"
#include "Pokemon/PokemonDTO.h"

#include "Move.generated.h"

struct FMoveTarget;
struct FMoveData;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class POKEMONCORE_API UMove : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface for a basic Pokémon move
 */
class POKEMONCORE_API IMove {
    GENERATED_BODY()

  public:
    virtual FMoveDTO ToDTO() const = 0;

    /**
     * Get the data for the underlying move that this move represents
     *
     * @return The data for this move
     */
    virtual const FMoveData &GetMoveData() const = 0;

    /**
     * Get the display name of the move
     *
     * @return The move's display name
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual FText GetDisplayName() const = 0;

    /**
     * Retrieve the description of the move.
     *
     * @return The move's description text.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual FText GetMoveDescription() const = 0;

    /**
     * Get the type of the move
     *
     * @return The move's type
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual FName GetType() const = 0;

    /**
     * Get the damage category of the move
     *
     * @return The damage category for the given move
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual EMoveDamageCategory GetDamageCategory() const = 0;

    /**
     * Get the base power for the move in question
     *
     * @return The move's base power
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual int32 GetBasePower() const = 0;

    /**
     * Check if the move has variable power.
     *
     * @return true if the move's power varies, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual bool IsVariablePower() const = 0;

    /**
     * Get the accuracy of the move in question
     *
     * @return The move's accuracy
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual int32 GetAccuracy() const = 0;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual bool AlwaysHits() const = 0;

    /**
     * Get the current PP for the move
     *
     * @return The current PP for the move
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual int32 GetCurrentPP() const = 0;

    /**
     * Get the total PP for this move
     *
     * @return The total PP for the move
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual int32 GetTotalPP() const = 0;

    /**
     * Lookup the function code of the move in question.
     *
     * @return The string used to denote the move's effect
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual FName GetFunctionCode() const = 0;

    /**
     * Get the target type of the move
     *
     * @return The target of the move in question (None = the move calculates the target)
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual const FMoveTarget &GetTargetType() const = 0;

    /**
     * Get the move's additional effect chance
     *
     * @return The move's additional effect chance
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual int32 GetAdditionalEffectChance() const = 0;

    /**
     * Get the tags of the move
     *
     * @return Words/phrases that can be used to group certain kinds of moves.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual const TArray<FName> &GetTags() const = 0;

    /**
     * Increase the amount of PP by the set provided amount.
     *
     * @param Amount The amount of PP to recover
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual void RecoverPP(int32 Amount = 1) = 0;

    /**
     * Reduce the amount of PP a move has by the given amount
     *
     * @param Amount The amount of PP to reduce by
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    virtual void DecrementPP(int32 Amount = 1) = 0;
};
