// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageCategory.h"
#include "UObject/Object.h"
#include "Move.generated.h"

/**
 * Represents the data for a Move in the game
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FMove : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * ID of this move
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ID;

	/**
	 * Name of this move as displayed by the game.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealName;

	/**
	 * The type of the move
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Type;

	/**
	 * The damage category of the move
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EDamageCategory Category;

	/**
	 * The Base Power of the move (0 = non-damaging, 1 = unknown/fixed damage)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, UIMin = 0))
	int32 Power;

	/**
	 * The accuracy of the move in question (0 = always hits)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 Accuracy;

	/**
	 * The max PP that the move can have by default
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 1, UIMin = 1))
	int32 TotalPP;

	/**
	 * The priority of the move.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Priority;

	/**
	 * The target of the move in question (None = the move calculates the target)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString Target;

	/**
	 * The function code of the move used to locate the move effect during runtime
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName FunctionCode;

	/**
	 * Words/phrases that can be used to group certain kinds of moves."
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Tags;

	/**
	 * The chance of the moves additional effect happening (0 = guaranteed to occur)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 EffectChance;

	/**
	 * Description of this item.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Description;
};
