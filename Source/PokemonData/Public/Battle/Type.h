// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Type.generated.h"

/**
 * Represents the types that belong to both individual species as well as the moves used by them
 */
USTRUCT(BlueprintType, meta = (DatabaseType="PBS"))
struct POKEMONDATA_API FType : public FTableRowBase {
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
	 * Graphic that represents a sprite in the menu
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UPaperSprite* Icon;

	/**
	 * Is this type special? (Only means anything when the Physical/Special-split is disabled)
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool IsSpecialType = false;

	/**
	 * Is this type only a type used by moves amd not one that a Pokémon can be.
	 *
	 * Examples of this would include the "???" Type used by Curse in generation II-IV and Shadow Moves.
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool IsPseudoType = false;

	/**
	 * Move types that deal super-effective damage to this type
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Weaknesses;

	/**
	 * Move types that deal not very effective damage to this type
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Resistances;

	/**
	 * Move types that deal no damage to this type
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Immunities;

	/**
	 * Words/phrases that can be used to group certain kinds of types"
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Tags;
};
