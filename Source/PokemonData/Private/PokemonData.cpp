#include "PokemonData.h"

#include "Bag/Item.h"
#include "Battle/BattleTerrain.h"
#include "Battle/BattleWeather.h"
#include "Battle/Environment.h"
#include "Battle/Status.h"
#include "DataRetrieval/DataRegistry.h"
#include "Exp/GrowthRateData.h"
#include "Field/EncounterType.h"
#include "Field/Weather.h"
#include "Moves/Target.h"
#include "Species/BodyColor.h"
#include "Species/BodyShape.h"
#include "Species/EggGroup.h"
#include "Species/EvolutionData.h"
#include "Species/GenderRatio.h"
#include "Species/Habitat.h"
#include "Species/Nature.h"
#include "Species/Stat.h"

constexpr auto GLoctextNamespace = "FPokemonDataModule";

void FPokemonDataModule::StartupModule() {
	auto &DataRegistry = FDataRegistry::GetInstance();
	DataRegistry.RegisterStruct<FGrowthRateData>();
	DataRegistry.RegisterStruct<FGenderRatio>();
	DataRegistry.RegisterStruct<FEggGroup>();
	DataRegistry.RegisterStruct<FBodyShape>();
	DataRegistry.RegisterStruct<FBodyColor>();
	DataRegistry.RegisterStruct<FHabitat>();
	DataRegistry.RegisterStruct<FEvolutionData>();
	DataRegistry.RegisterStruct<FStat>();
	DataRegistry.RegisterStruct<FNature>();
	DataRegistry.RegisterStruct<FStatus>();
	DataRegistry.RegisterStruct<FWeather>();
	DataRegistry.RegisterStruct<FEncounterType>();
	DataRegistry.RegisterStruct<FEnvironment>();
	DataRegistry.RegisterStruct<FBattleWeather>();
	DataRegistry.RegisterStruct<FBattleTerrain>();
	DataRegistry.RegisterStruct<FTarget>();

	DataRegistry.RegisterStruct<FItem>();
}

void FPokemonDataModule::ShutdownModule() {
	// No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
