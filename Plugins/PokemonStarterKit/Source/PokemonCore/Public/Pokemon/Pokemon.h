// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Breeding/PokemonGender.h"
#include "Manipulation/CircularIterator.h"
#include "Trainers/Trainer.h"
#include "UObject/Interface.h"

#include "Pokemon.generated.h"

struct FStatus;
class IObtainedBlock;
struct FOwnerInfo;
class IAbilityBlock;
struct FAbility;
class IMoveBlock;
struct FPokemonDTO;
class IStatBlock;
struct FSpeciesData;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType, meta = (Injectable))
class POKEMONCORE_API UPokemon : public UInterface {
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
    virtual void Initialize(const FPokemonDTO &DTO, const TScriptInterface<ITrainer> &Trainer = nullptr) = 0;

    virtual FPokemonDTO ToDTO() const = 0;

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
     * Get the types of the Pokémon in question
     * @return The Pokémon's type(s)
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual const TArray<FName> &GetTypes() const = 0;

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

    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual bool IsAble() const = 0;

    /**
     * Get the current HP of the Pokémon in question
     * @return The current HP of this particular Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetCurrentHP() const = 0;

    /**
     * Set the Pokémon's current HP to the given value
     * @param Value The value of HP for the Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual void SetCurrentHP(int32 Value) = 0;

    /**
     * Get the current HP of the Pokémon in question
     * @return The current HP of this particular Pokémon
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 GetMaxHP() const = 0;

    /**
     * Restore HP by the specified amount.
     * @param Amount The amount of HP to restore.
     * @return The actual amount of HP recovered.
     */
    UFUNCTION(BlueprintCallable, Category = Stats)
    virtual int32 RestoreHP(int32 Amount) = 0;

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
     * Set the item that the Pokémon is currently holding
     * @param Item The currently held item
     */
    UFUNCTION(BlueprintCallable, Category = Items)
    virtual void SetHoldItem(FName Item) = 0;

    /**
     * Remove the Pokémon's held item if it has one
     */
    UFUNCTION(BlueprintCallable, Category = Items)
    virtual void RemoveHoldItem() = 0;

    /**
     * Get the status effect of the Pokémon.
     * @return The status effect of the Pokémon
     */
    virtual const FStatus *GetStatusEffect() const = 0;

    /**
     * Set the Pokémon's status effect
     * @param StatusID The ID of the status effect in question.
     * @param bOverwriteExisting Should this overwrite the existing status effect?
     * @return Was the status effect changed?
     */
    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    virtual bool SetStatusEffect(FName StatusID, bool bOverwriteExisting = false) = 0;

    /**
     * Remove a Pokémon's status condition
     */
    UFUNCTION(BlueprintCallable, Category = StatusEffects)
    virtual void RemoveStatusEffect() = 0;

    /**
     * Get the information about the Pokémon's original trainer
     * @return Is information about the original trainer
     */
    UFUNCTION(BlueprintCallable, Category = Trainer)
    virtual const FOwnerInfo &GetOwnerInfo() const = 0;

    /**
     * Get the information about how this Pokémon was obtained.
     * @return How was this Pokémon obtained?
     */
    UFUNCTION(BlueprintCallable, Category = Trainer)
    virtual TScriptInterface<IObtainedBlock> GetObtainedInformation() const = 0;

    /**
     * Get the information about the Pokémon's current handler
     * @return Is information about the current handler
     */
    UFUNCTION(BlueprintCallable, Category = Trainer)
    virtual const TScriptInterface<ITrainer> &GetCurrentHandler() const = 0;

    UFUNCTION(BlueprintCallable, Category = Trainer)
    virtual void SetCurrentHandler(const TScriptInterface<ITrainer> &NewHandler) = 0;
};

/**
 * Circular iterator specifically for Pokémon in the party or a box
 */
using FPokemonIterator = TCircularIterator<TScriptInterface<IPokemon>>;