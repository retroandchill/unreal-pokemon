// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "ConstructionUtilities.generated.h"

class IStatBlock;
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

	/**
	 * Creat the stat block based on the settings in the
	 * @param Owner The Growth Rate type of the Pokémon
	 * @param DTO The DTO to copy any saved data from
	 * @return The created block
	 */
	static TScriptInterface<IStatBlock> CreateStatBlock(const TScriptInterface<IPokemon>& Owner, const FPokemonDTO &DTO);
};
