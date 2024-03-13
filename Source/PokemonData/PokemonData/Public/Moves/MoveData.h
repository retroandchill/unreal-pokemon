// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "MoveDamageCategory.h"
#include "UObject/Object.h"
#include "MoveData.generated.h"

/**
 * Represents the data for a Move in the game
 */
USTRUCT(BlueprintType, meta = (DatabaseType="PBS"))
struct POKEMONDATA_API FMoveData : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * The internal ID used for lookup by the game
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealName;

	/**
	 * Description as displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText Description;

	/**
	 * The type of the move
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Classification")
	FName Type;

	/**
	 * The damage category of the move
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Classification")
	EMoveDamageCategory Category;

	/**
	 * The Base Power of the move (0 = non-damaging, 1 = unknown/fixed damage)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats", meta = (ClampMin = 0, UIMin = 0))
	int32 Power;

	/**
	 * The accuracy of the move in question (0 = always hits)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats",
		meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 Accuracy;

	/**
	 * The max PP that the move can have by default
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats", meta = (ClampMin = 1, UIMin = 1))
	int32 TotalPP;

	/**
	 * The priority of the move.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Usage")
	int32 Priority;

	/**
	 * The target of the move in question (None = the move calculates the target)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Battle Usage")
	FString Target;

	/**
	 * The function code of the move used to locate the move effect during runtime
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect")
	FName FunctionCode;

	/**
	 * The chance of the moves additional effect happening (0 = guaranteed to occur)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effect",
		meta = (ClampMin = 0, ClampMax = 100, UIMin = 0, UIMax = 100))
	int32 EffectChance;

	/**
	 * Words/phrases that can be used to group certain kinds of moves."
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
	TArray<FName> Tags;
};
