// "Unreal Pokémon" created by Retro & Chill.


#include "Managers/PokemonSubsystem.h"

#include "PokemonCoreSettings.h"
#include "Trainers/TrainerStub.h"

UPokemonSubsystem *UPokemonSubsystem::Instance = nullptr;

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	Instance = this;
	auto Settings = GetDefault<UPokemonCoreSettings>();
	HPStat = Settings->GetHPStat();
	MaxPartySize = Settings->GetMaxPartySize();
	
	// TODO: Swap this instantiation with the actual trainer instantiation
	Player = MakeUnique<FTrainerStub>();
}

void UPokemonSubsystem::Deinitialize() {
	Super::Deinitialize();
	Instance = nullptr;
}

UPokemonSubsystem& UPokemonSubsystem::GetInstance() {
	check(Instance != nullptr)
	return *Instance;
}

FName UPokemonSubsystem::GetHPStat() const {
	return HPStat;
}

int32 UPokemonSubsystem::GetMaxPartySize() const {
	return MaxPartySize;
}

ITrainer& UPokemonSubsystem::GetPlayer() {
	check(Player != nullptr)
	return *Player;
}

const ITrainer& UPokemonSubsystem::GetPlayer() const {
	check(Player != nullptr)
	return *Player;
}
