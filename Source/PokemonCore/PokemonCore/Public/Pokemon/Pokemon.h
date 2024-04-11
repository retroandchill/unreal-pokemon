// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Breeding/PokemonGender.h"
#include "CoreMinimal.h"
#include "Meta/MetatypeDeclares.h"
#include "Meta/PokeRegistry.h"
#include "Pokemon.generated.h"
#include "UObject/Interface.h"

struct FPokemonDTO;
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
     * Construct a Pokémon from the DTO
     * @param DTO The source Pokémon DTO to initialize from
     */
    virtual void Initialize(const FPokemonDTO &DTO) = 0;

    /**
     * Get the name of the Pokémon in question
     * @return The Pokémon's Nickname
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual FText GetNickname() const = 0;

    /**
     * Get the Pokémon's Personality value
     * @return The internal personality value of the Pokémon. Determines the default values of various aspects of the
     * Pokémon if the values are not already set.
     */
    virtual uint32 GetPersonalityValue() const = 0;

    /**
     * Get the species information about the Pokémon in question
     * @return The species data
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual const FSpeciesData &GetSpecies() const = 0;

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
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual TScriptInterface<IStatBlock> GetStatBlock() const = 0;
};