// "Unreal Pokémon" created by Retro & Chill.


#include "Managers/PokemonSubsystem.h"

#include "Asserts.h"
#include "PokemonCoreSettings.h"
#include "Player/PlayerMetadata.h"
#include "Trainers/TrainerStub.h"

UPokemonSubsystem* UPokemonSubsystem::Instance = nullptr;

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	Instance = this;
	auto Settings = GetDefault<UPokemonCoreSettings>();
	HPStat = Settings->GetHPStat();
	MaxPartySize = Settings->GetMaxPartySize();

	StartNewGame();
}

void UPokemonSubsystem::Deinitialize() {
	Super::Deinitialize();
	Instance = nullptr;
}

UPokemonSubsystem& UPokemonSubsystem::GetInstance() {
	ASSERT(Instance != nullptr)
	return *Instance;
}

void UPokemonSubsystem::StartNewGame() {
	// TODO: Swap this instantiation with the actual trainer instantiation
	Player = MakeUnique<FTrainerStub>();
	PlayerMetadata = NewObject<UPlayerMetadata>();
	PlayerMetadata->StartNewGame();
}

FName UPokemonSubsystem::GetHPStat() const {
	return HPStat;
}

int32 UPokemonSubsystem::GetMaxPartySize() const {
	return MaxPartySize;
}

ITrainer& UPokemonSubsystem::GetPlayer() {
	ASSERT(Player != nullptr)
	return *Player;
}

const ITrainer& UPokemonSubsystem::GetPlayer() const {
	ASSERT(Player != nullptr)
	return *Player;
}

const UPlayerMetadata& UPokemonSubsystem::GetPlayerMetadata() const {
	return *PlayerMetadata;
}
