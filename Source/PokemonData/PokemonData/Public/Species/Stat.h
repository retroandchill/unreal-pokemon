// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PokemonStatType.h"
#include "UObject/Object.h"
#include "Stat.generated.h"

/**
 * Represents one of the stats in the database.
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FStat : public FTableRowBase {
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
	 * The name that is displayed to the player when there is a limited space
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
	FText RealNameBrief;

	/**
	 * What kind of stat is this?
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	EPokemonStatType Type;

	/**
	 * The order this stat is processed when parsing the PBS files
	 */
	UPROPERTY(DisplayName = "PBS Order", BlueprintReadOnly, EditAnywhere, Category = "Metadata",
		meta = (EditCondition = "Type != EPokemonStatType::Battle", UIMin = 0, ClampMin = 0))
	int32 PbsOrder;
};
