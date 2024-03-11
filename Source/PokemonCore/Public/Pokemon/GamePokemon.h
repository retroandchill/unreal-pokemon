// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "Memory/RowPointer.h"
#include "Stats/StatBlock.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"

class IMove;

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
class POKEMONCORE_API FGamePokemon : public IPokemon {
public:
	/**
	 * Create a Pokémon with the following species and level information
	 * @param SpeciesID The species of Pokémon to create
	 * @param Level The level of the Pokémon in question
	 */
	explicit FGamePokemon(FName SpeciesID, int32 Level = 5);

	/**
	 * Create a new Pokémon with all of its data explicitly defined
	 * @param SpeciesID The species of Pokémon to create
	 * @param Level The level of the Pokémon in question
	 * @param Gender The Pokémon's gender
	 * @param IVs The IVs of the Pokémon
	 * @param EVs The EVs of the Pokémon
	 * @param Nature The Pokémon's Nature
	 * @param Ability The index of the Pokémon's ability
	 * @param Moves The moves the Pokémon has
	 * @param Shiny Is the Pokémon in question shiny
	 * @param Item The item the Pokémon is holding
	 */
	FGamePokemon(FName SpeciesID, int32 Level, EPokemonGender Gender, const TMap<FName, int32>& IVs, const TMap<FName, int32>& EVs,
					  FName Nature, int32 Ability, TArray<TSharedRef<IMove>> &&Moves, bool Shiny, FName Item = FName());

	FGamePokemon(FGamePokemon&& Other) noexcept = default;

	FGamePokemon& operator=(FGamePokemon&& Other) noexcept = default;

	FText GetName() const override;
	const FSpeciesData& GetSpecies() const override;
	EPokemonGender GetGender() const override;
	int32 GetCurrentHP() const override;
	int32 GetMaxHP() const override;
	bool IsFainted() const override;
	const IStatBlock& GetStatBlock() const override;

private:
	/**
	 * Perform any common initialization steps
	 */
	void CommonInit();
	
	/**
	 * The ID of the species of Pokémon this is
	 */
	TRowPointer<FSpeciesData> Species;

	/**
	 * The internal personality value of the Pokémon. Determines the default values of various aspects of the
	 * Pokémon if the values are not already set.
	 */
	uint32 PersonalityValue = UPersonalityValueUtils::GeneratePersonalityValue();

	/**
	 * The nickname assigned to the Pokémon. Uses the species name if empty.
	 */
	TOptional<FText> Nickname;

	/**
	 * The hardcoded gender of the Pokémon. Calculates using the personality value is unset.
	 */
	TOptional<EPokemonGender> Gender;

	/**
	 * The hardcoded shiny status of the Pokémon. Calculates using the personality value is unset.
	 */
	TOptional<bool> Shiny;

	/**
	 * The current amount of HP this Pokémon has
	 */
	int32 CurrentHP;

	/**
	 * The handler for calculating stats
	 */
	TUniquePtr<IStatBlock> StatBlock;

	/**
	 * The moves this Pokémon knows
	 */
	TArray<TSharedRef<IMove>> Moves;
};
