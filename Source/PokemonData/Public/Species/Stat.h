// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonStatType.h"
#include "UObject/Object.h"
#include "Stat.generated.h"

/**
 * Represents one of the stats in the database
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FStat : public FTableRowBase {
	GENERATED_BODY()

	/**
	 * The internal ID used for lookup by the game
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName ID;

	/**
	 * The name that is displayed to the player
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealName;

	/**
	 * The name that is displayed to the player when there is a limited space
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText RealNameBrief;

	/**
	 * What kind of stat is this?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EPokemonStatType Type;

	/**
	 * The order this stat is processed when parsing the PBS files
	 */
	UPROPERTY(DisplayName = "PBS Order", BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "Type != EPokemonStatType::Battle", UIMin = 0, ClampMin = 0))
	int32 PbsOrder;
};
