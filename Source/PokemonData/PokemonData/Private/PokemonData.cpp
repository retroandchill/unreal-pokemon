// "Unreal Pokémon" created by Retro & Chill.
#include "PokemonData.h"

#include "PokemonDataSettings.h"
#include "Bag/Item.h"
#include "Battle/BattleTerrain.h"
#include "Battle/BattleWeather.h"
#include "Battle/Environment.h"
#include "Battle/Status.h"
#include "Battle/Type.h"
#include "DataRetrieval/DataRegistry.h"
#include "Exp/GrowthRateData.h"
#include "Field/EncounterType.h"
#include "Field/Weather.h"
#include "..\Public\Moves\MoveData.h"
#include "Moves/Target.h"
#include "Species/Ability.h"
#include "Species/BodyColor.h"
#include "Species/BodyShape.h"
#include "Species/EggGroup.h"
#include "Species/EvolutionData.h"
#include "Species/GenderRatio.h"
#include "Species/Habitat.h"
#include "Species/Nature.h"
#include "..\Public\Species\SpeciesData.h"
#include "Species/Stat.h"
#include "Trainers/TrainerType.h"

constexpr auto GLoctextNamespace = "FPokemonDataModule";

void FPokemonDataModule::StartupModule() {
	auto& DataRegistry = FDataRegistry::GetInstance();
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

	DataRegistry.RegisterStruct<FType>();
	DataRegistry.RegisterStruct<FAbility>();
	DataRegistry.RegisterStruct<FMoveData>();
	DataRegistry.RegisterStruct<FItem>();
	DataRegistry.RegisterStruct<FSpeciesData>();
	DataRegistry.RegisterStruct<FTrainerType>();

	auto Settings = GetDefault<UPokemonDataSettings>();
	for (auto &DataTables = Settings->GetDataTables(); auto &Table : DataTables) {
		Table.TryLoad();
	}
}

void FPokemonDataModule::ShutdownModule() {
	// No custom shutdown needed
}

IMPLEMENT_MODULE(FPokemonDataModule, PokemonData)
