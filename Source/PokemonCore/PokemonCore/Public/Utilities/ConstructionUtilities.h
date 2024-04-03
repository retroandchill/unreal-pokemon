// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "ConstructionUtilities.generated.h"

struct FPokemonDTO;
class IPokemon;

/**
 * Utility class used for constructing game objects, with specific arguments.
 */
UCLASS()
class POKEMONCORE_API UConstructionUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Create a new Pokémon from the provided data.
	 * @param Data The data used to construct the Pokémon.
	 * @return The created Pokémon.
	 */
	UFUNCTION(BlueprintCallable, Category = "Object|Constructors")
	static TScriptInterface<IPokemon> CreateNewPokemon(const FPokemonDTO& Data);
};
