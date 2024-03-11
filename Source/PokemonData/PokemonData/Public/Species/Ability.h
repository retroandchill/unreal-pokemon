// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Ability.generated.h"

/**
 * Represents the data for an Ability type in the game
 */
USTRUCT(BlueprintType, meta = (DatabaseType="PBS"))
struct POKEMONDATA_API FAbility : public FTableRowBase {
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
	 * Words/phrases that can be used to group the data"
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
	TArray<FName> Tags;
};
