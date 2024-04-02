// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Breeding/PokemonGender.h"
#include "Meta/MetatypeDeclares.h"
#include "Meta/PokeRegistry.h"
#include "UObject/Interface.h"
#include "Pokemon.generated.h"

struct FPokemonDTO;
class UPokemonBuilder;
class IStatBlock;
struct FSpeciesData;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UPokemon : public UInterface {
	GENERATED_BODY()
};

/**
 * Abstract interface for a Pokémon object
 */
class POKEMONCORE_API IPokemon {
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Get the name of the Pokémon in question
	 * @return The Pokémon's Nickname
	 */
	UFUNCTION(BlueprintCallable, Category = Bio)
	virtual FText GetNickname() const = 0;

	/**
	 * Get the species information about the Pokémon in question
	 * @return The species data
	 */
	UFUNCTION(BlueprintCallable, Category = Bio)
	virtual const FSpeciesData& GetSpecies() const = 0;

	/**
	 * Get the gender of the Pokémon in question
	 * @return The Pokémon's gender
	 */
	UFUNCTION(BlueprintCallable, Category = Bio)
	virtual EPokemonGender GetGender() const = 0;

	/**
	 * Get the current HP of the Pokémon in question
	 * @return The current HP of this particular Pokémon
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual int32 GetCurrentHP() const = 0;

	/**
	 * Get the current HP of the Pokémon in question
	 * @return The current HP of this particular Pokémon
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual int32 GetMaxHP() const = 0;

	/**
	 * Returns if the Pokémon is currently fainted
	 * @return Is the Pokémon currently fainted
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual bool IsFainted() const = 0;

	/**
	 * Get the stat information for this Pokémon
	 * @return A reference to the owned stat block
	 */
	//UFUNCTION(BlueprintCallable, Category = Stats)
	virtual const IStatBlock& GetStatBlock() const = 0;

	/**
	 * Convert this class into a builder object
	 * @return The builder class to use
	 */
	UFUNCTION(BlueprintCallable, Category = Stats)
	virtual UPokemonBuilder *ToBuilder() const = 0;

	/**
	 * Check if two Pokémon are the same
	 * @param Other The other Pokémon
	 * @return Are the two Pokémon the same?
	 */
	virtual bool operator==(const IPokemon& Other) const = 0;
};