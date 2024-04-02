// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Habitat.generated.h"

/**
 * Represents the Body Shape of a Pokémon. Used when searching in the Pokédex.
 */
USTRUCT(BlueprintType, meta = (DatabaseType="Hardcoded"))
struct POKEMONDATA_API FHabitat : public FTableRowBase {
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
};

/**
 * Blueprint function library for getting habitat data out.
 */
UCLASS()
class POKEMONDATA_API UHabitatHelper : public UBlueprintFunctionLibrary { 
	GENERATED_BODY() 

public:
	/**
	 * Get the list of all possible habitat names.
	 * @return The list of all possible habitat names.
	 */
	UFUNCTION() 
	static TArray<FName> GetHabitatNames(); 
};