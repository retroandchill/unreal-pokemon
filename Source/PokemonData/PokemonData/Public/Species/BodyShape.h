// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "BodyShape.generated.h"

/**
 * Represents the Body Shape of a Pokémon. Used when searching in the Pokédex.
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FBodyShape : public FTableRowBase {
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
	 * The sprite used as the Icon for this graphic
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals")
	UPaperSprite* Icon;
};
