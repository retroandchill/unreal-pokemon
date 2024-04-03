// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "NatureStatChange.h"
#include "UObject/Object.h"
#include "Nature.generated.h"

/**
 * Represents one of the possible Pokémon natures
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FNature : public FTableRowBase {
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
	 * The list of stat changes to be applied
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
	TArray<FNatureStatChange> StatChanges;
};