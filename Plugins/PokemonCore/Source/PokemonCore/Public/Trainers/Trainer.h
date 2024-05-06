// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

#include "Trainer.generated.h"

struct FTrainerType;
class IPokemon;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UTrainer : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface that defines the basic functionality of how a Trainer should behave.
 */
class POKEMONCORE_API ITrainer {
    GENERATED_BODY()

  public:
    /**
     * Initialize a new trainer with the following type and name
     * @param NewTrainerType The trainer class to use
     * @param NewTrainerName The name of the trainer
     * @return A pointer to this trainer
     */
    virtual TScriptInterface<ITrainer> Initialize(FName NewTrainerType, FText NewTrainerName) = 0;

    /**
     * Get the type of the trainer this is
     * @return The type of trainer this is
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual const FTrainerType &GetTrainerType() const = 0;

    /**
     * Get the name of the trainer
     * @return The trainer's name
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual FText GetTrainerName() const = 0;

    /**
     * Get the amount of money this trainer will pay out
     * @return The amount of money this trainer pays out
     */
    UFUNCTION(BlueprintCallable, Category = Drops)
    virtual int32 GetPayout() const = 0;

    /**
     * Get the trainer's list of party Pokémon
     * @return The list of Pokémon in the order the trainer has them in
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual const TArray<TScriptInterface<IPokemon>> &GetParty() const = 0;

    /**
     * Add a Pokémon to the trainer's party
     * @param Pokemon The Pokémon to add to the party
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual void AddPokemonToParty(const TScriptInterface<IPokemon> &Pokemon) = 0;

    /**
     * Switch the positions of two Pokémon in the party
     * @param Index1 The index of the first Pokémon to switch
     * @param Index2 The index of the second Pokémon to switch
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual void SwapPositionsInParty(int32 Index1, int32 Index2) = 0;

    /**
     * Remove all Pokémon from the player's party
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual void ClearParty() = 0;

    /**
     * Get the trainer's ID number
     * @return The trainer's ID number
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual int32 GetIdNumber() = 0;

    /**
     * Get the trainer's secret ID number
     * @return The trainer's secret ID number
     */
    UFUNCTION(BlueprintCallable, Category = Bio)
    virtual int32 GetSecretId() = 0;
};
