// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Breeding/PokemonGender.h"
#include "Trainers/Trainer.h"
#include "UObject/Interface.h"

#include "Pokemon.generated.h"

struct FOwnerInfo;
class IAbilityBlock;
struct FAbility;
class IMoveBlock;
struct FPokemonDTO;
class IStatBlock;
struct FSpeciesData;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
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
     * @param Trainer
     */
    virtual void Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer>& Trainer = nullptr) = 0;

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
     * Get the Poké Ball that the Pokémon was caught in
     * @return The Poké Ball caught in
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual FName GetPokeBall() const = 0;

    /**
     * Get if the Pokémon is shiny or not
     * @return Is the Pokémon shiny?
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual bool IsShiny() const = 0;

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

    /**
     * Get the move information for this Pokémon
     * @return A reference to the owned move block
     */
    UFUNCTION(BlueprintCallable, Category = Moves)
    virtual TScriptInterface<IMoveBlock> GetMoveBlock() const = 0;

    /**
     * Get the information for the Ability possessed by this Pokémon
     * @return The ability this Pokémon has
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual TScriptInterface<IAbilityBlock> GetAbility() const = 0;

    /**
     * Get the hold item used by the Pokémon.
     * @return The held item used by the given Pokémon if it has one
     */
    virtual const FItem *GetHoldItem() const = 0;

    /**
     * Get the information about the Pokémon's original trainer
     * @return Is information about the original trainer
     */
    UFUNCTION(BlueprintCallable, Category = Trainer)
    virtual const FOwnerInfo &GetOwnerInfo() const = 0;
};