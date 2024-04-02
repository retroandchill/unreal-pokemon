// "Unreal Pokémon" created by Retro & Chill.


#include "Managers/PokemonSubsystem.h"

#include "Asserts.h"
#include "PokemonCoreSettings.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Trainers/TrainerStub.h"

UPokemonSubsystem* UPokemonSubsystem::Instance = nullptr;

void UPokemonSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	Instance = this;
	auto Settings = GetDefault<UPokemonCoreSettings>();
	HPStat = Settings->GetHPStat();
	MaxPartySize = Settings->GetMaxPartySize();
	
	GrowthRates.Empty();
	for (auto RegisteredTypes = Exp::FGrowthRateRegistry::GetInstance().GetAllRegisteredTypes();
		auto Type : RegisteredTypes) {
		GrowthRates.Add(Type, Exp::FGrowthRateRegistry::GetInstance().Construct(Type));
	}

	// TODO: Swap this instantiation with the actual trainer instantiation
	Player = NewObject<UTrainerStub>(this)->Initialize();
}

void UPokemonSubsystem::Deinitialize() {
	Super::Deinitialize();
	Instance = nullptr;
}

UPokemonSubsystem& UPokemonSubsystem::GetInstance() {
	ASSERT(Instance != nullptr)
	return *Instance;
}

bool UPokemonSubsystem::Exists() {
	return Instance != nullptr;
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

const Exp::IGrowthRate& UPokemonSubsystem::GetGrowthRate(FName GrowthRate) const {
	ASSERT(GrowthRates.Contains(GrowthRate));
	return *GrowthRates[GrowthRate];
}
