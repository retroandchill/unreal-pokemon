// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon.h"
#include "Memory/RowPointer.h"
#include "Stats/StatBlock.h"
#include "Species/SpeciesData.h"
#include "Utilities/PersonalityValueUtils.h"
#include "GamePokemon.generated.h"

struct FPokemonDTO;
class IMove;

/**
 * Basic Pokémon class that holds all of the information for a complete Pokémon
 */
UCLASS()
class POKEMONCORE_API UGamePokemon : public UObject, public IPokemon {
	GENERATED_BODY()
	
public:
	/**
	 * Construct a Pokémon from the DTO
	 * @param DTO The source Pokémon DTO to initialize from
	 */
	UGamePokemon *Initialize(const FPokemonDTO& DTO);

	
	UFUNCTION(BlueprintPure, Category = Bio)
	FText GetNickname() const override;

	UFUNCTION(BlueprintPure, Category = Bio)
	const FSpeciesData& GetSpecies() const override;

	UFUNCTION(BlueprintPure, Category = Bio)
	EPokemonGender GetGender() const override;

	UFUNCTION(BlueprintPure, Category = Bio)
	int32 GetCurrentHP() const override;
	
	UFUNCTION(BlueprintPure, Category = Bio)
	int32 GetMaxHP() const override;

	UFUNCTION(BlueprintPure, Category = Bio)
	bool IsFainted() const override;

	//UFUNCTION(BlueprintPure, Category = Bio)
	const IStatBlock& GetStatBlock() const override;

	UFUNCTION(BlueprintPure, Category = Bio)
	UPokemonBuilder* ToBuilder() const override;

	bool operator==(const IPokemon& Other) const override;

	/**
	 * Check if two Pokémon are the same
	 * @param Other The other Pokémon
	 * @return Are the two Pokémon the same?
	 */
	bool operator==(const UGamePokemon& Other) const;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	UPROPERTY(SaveGame)
	FSpeciesDataPointer Species;

	/**
	 * The internal personality value of the Pokémon. Determines the default values of various aspects of the
	 * Pokémon if the values are not already set.
	 */
	UPROPERTY(SaveGame)
	uint32 PersonalityValue;

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
