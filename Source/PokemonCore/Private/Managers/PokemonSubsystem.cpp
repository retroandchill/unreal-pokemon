// "Unreal Pokémon" created by Retro & Chill.


#include "Managers/PokemonSubsystem.h"

#include "Trainers/TrainerStub.h"

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	
	// TODO: Swap this instantiation with the actual trainer instantiation
	Player = MakeUnique<FTrainerStub>();
}

UPokemonSubsystem& UPokemonSubsystem::GetInstance() {
	check(GEngine != nullptr);
	
	auto World = GEngine->GetWorld();
	check(World != nullptr);
	
	auto GameInstance = World->GetGameInstance();
	check(GameInstance != nullptr);
	
	return *GameInstance->GetSubsystem<UPokemonSubsystem>();
}

FName UPokemonSubsystem::GetHPStat() const {
	return HPStat;
}

ITrainer& UPokemonSubsystem::GetPlayer() {
	check(Player != nullptr);
	return *Player;
}

const ITrainer& UPokemonSubsystem::GetPlayer() const {
	check(Player != nullptr);
	return *Player;
}
