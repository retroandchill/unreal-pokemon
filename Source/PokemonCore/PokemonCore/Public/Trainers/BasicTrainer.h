﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Trainer.h"
#include "Trainers/TrainerType.h"
#include "BasicTrainer.generated.h"

/**
 * The basic default implementation details for a trainer
 */
UCLASS()
class POKEMONCORE_API UBasicTrainer : public UObject, public ITrainer {
	GENERATED_BODY()
	
public:
	TScriptInterface<ITrainer> Initialize(FName NewTrainerType, FText NewTrainerName) override;
	
	UFUNCTION(BlueprintPure, Category = Bio)
	const FTrainerType& GetTrainerType() const override;
	
	UFUNCTION(BlueprintPure, Category = Bio)
	FText GetTrainerName() const override;
	
	UFUNCTION(BlueprintPure, Category = Drops)
	int32 GetPayout() const override;
	
	UFUNCTION(BlueprintPure, Category = "Pokémon")
	const TArray<TScriptInterface<IPokemon>>& GetParty() const override;

	UFUNCTION(BlueprintCallable, Category = "Pokémon")
	void AddPokemonToParty(const TScriptInterface<IPokemon>& Pokemon) override;

	UFUNCTION(BlueprintCallable, Category = "Pokémon")
	void SwapPositionsInParty(int32 Index1, int32 Index2) override;

	/**
	 * Remove all Pokémon from the player's party
	 */
	UFUNCTION(BlueprintCallable, Category = "Pokémon")
	void ClearParty() override;
	
	UFUNCTION(BlueprintPure, Category = Bio)
	int32 GetIdNumber() override;
	
	UFUNCTION(BlueprintPure, Category = Bio)
	int32 GetSecretId() override;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	UPROPERTY(SaveGame)
	FName TrainerType;
	
	/**
	 * The trainer's name
	 */
	UPROPERTY(SaveGame)
	FText Name;
	
	/**
	 * The Pokémon in the trainer's party
	 */
	UPROPERTY(SaveGame)
	TArray<TScriptInterface<IPokemon>> Party;

	/**
	 * The trainer's ID number
	 */
	UPROPERTY(SaveGame)
	int32 ID;

	/**
	 * The trainer's secret ID number
	 */
	UPROPERTY(SaveGame)
	int32 SecretID;
};
