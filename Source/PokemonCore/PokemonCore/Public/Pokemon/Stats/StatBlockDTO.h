// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StatBlockDTO.generated.h"

/**
 * Data transfer object for the stat block
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FStatBlockDTO {
	GENERATED_BODY()

	/**
	 * The level of the created Pokémon
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	int32 Level;
	
	/**
	 * The amount of Exp the Pokémon has
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data, meta = (EditCondition=bOverride_Exp))
	int32 Exp;

	/**
	 * The override state of the exp of the Pokémon
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	bool bOverride_Exp;

	/**
	 * The IVs of the Pokémon to explicitly set
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	TMap<FName, int32> IVs;

	/**
	 * The EVs of the Pokémon to explicitly set
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	TMap<FName, int32> EVs;

	/**
	 * The Nature of the Pokémon in question
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data, meta = (EditCondition=bOverride_Nature))
	FName Nature;

	/**
	 * The override state of the nature of the Pokémon
	 */
	UPROPERTY(EditAnywhere, SaveGame, Category = Data)
	bool bOverride_Nature;
};
