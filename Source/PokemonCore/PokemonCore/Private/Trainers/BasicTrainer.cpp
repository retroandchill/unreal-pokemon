// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/BasicTrainer.h"

#include "DataManager.h"
#include "Pokemon/Pokemon.h"

/**
 * Helper function to locate the trainer type that this trainer refers to
 * @param TrainerType The ID of the trainer type to find
 * @return The located trainer typ data
 */
TRowPointer<FTrainerType> FindTrainerType(FName TrainerType) {
	const auto& DataManager = FDataManager::GetInstance();
	auto& SpeciesTable = DataManager.GetDataTable<FTrainerType>();

	auto SpeciesData = SpeciesTable.GetDataManaged(TrainerType);
	ASSERT(SpeciesData != nullptr)
	return SpeciesData;
}

FBasicTrainer::FBasicTrainer(FName TrainerType, FText Name) : TrainerType(FindTrainerType(TrainerType)), Name(Name),
	ID(FMath::RandRange(0, 999999)), SecretID(FMath::RandRange(0, 999999)) {
}

const FTrainerType& FBasicTrainer::GetTrainerType() const {
	return *TrainerType;
}

FText FBasicTrainer::GetName() const {
	return Name;
}

uint32 FBasicTrainer::GetPayout() const {
	GUARD(Party.IsEmpty(), 0)
	return TrainerType->BaseMoney * Party.Last()->GetStatBlock().GetLevel();
}

TArray<TSharedRef<IPokemon>>& FBasicTrainer::GetParty() {
	return Party;
}

const TArray<TSharedRef<IPokemon>>& FBasicTrainer::GetParty() const {
	return Party;
}

int32 FBasicTrainer::GetIdNumber() {
	return ID;
}

int32 FBasicTrainer::GetSecretId() {
	return SecretID;
}
