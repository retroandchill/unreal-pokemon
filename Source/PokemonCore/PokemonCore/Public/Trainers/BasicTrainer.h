// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Trainer.h"
#include "Memory/RowPointer.h"
#include "Trainers/TrainerType.h"

/**
 * The basic default implementation details for a trainer
 */
class POKEMONCORE_API FBasicTrainer : public ITrainer {
protected:
	FBasicTrainer(FName TrainerType, FText Name);

public:
	const FTrainerType& GetTrainerType() const override;
	FText GetName() const override;
	uint32 GetPayout() const override;
	TArray<TSharedRef<IPokemon>>& GetParty() override;
	const TArray<TSharedRef<IPokemon>>& GetParty() const override;
	int32 GetIdNumber() override;
	int32 GetSecretId() override;

private:
	/**
	 * The ID of the species of Pokémon this is
	 */
	TRowPointer<FTrainerType> TrainerType;
	
	/**
	 * The trainer's name
	 */
	FText Name;
	
	/**
	 * The Pokémon in the trainer's party
	 */
	TArray<TSharedRef<IPokemon>> Party;

	/**
	 * The trainer's ID number
	 */
	int32 ID;

	/**
	 * The trainer's secret ID number
	 */
	int32 SecretID;
};
