// Created by Retro & Chill.


#include "DataSubsystem.h"

#include "Exp/GrowthRateData.h"
#include "Species/GenderRatio.h"
#include "Species/EggGroup.h"
#include "Species/BodyShape.h"
#include "Species/BodyColor.h"
#include "Species/Habitat.h"
#include "Species/EvolutionData.h"
#include "Species/Stat.h"
#include "Species/Nature.h"
#include "Battle/Status.h"
#include "Field/Weather.h"
#include "Field/EncounterType.h"
#include "Battle/Environment.h"
#include "Battle/BattleWeather.h"
#include "Battle/BattleTerrain.h"
#include "Moves/Target.h"

#include "Bag/Item.h"

/**
 * Convenience method to load a data table into a proxy object
 * @tparam T The struct type the proxy stores
 * @param TableMap The map of types to tables
 * @param TableName The name of the table to load
 */
template <typename T>
static void LoadDataTable(TMap<TObjectPtr<const UScriptStruct>, TUniquePtr<IGameData>>& TableMap, FStringView TableName) {
	ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TableName.GetData());
	check(DataTable.Succeeded());

	TableMap.Add(T::StaticStruct(), MakeUnique<TDataTableProxy<T>>(DataTable.Object.Get()));
}

UDataSubsystem::UDataSubsystem() {
	LoadDataTable<FGrowthRateData>(DataTables, TEXT("/Game/Data/Hardcoded/GrowthRates.GrowthRates"));
	LoadDataTable<FGenderRatio>(DataTables, TEXT("/Game/Data/Hardcoded/GenderRatios.GenderRatios"));
	LoadDataTable<FEggGroup>(DataTables, TEXT("/Game/Data/Hardcoded/EggGroups.EggGroups"));
	LoadDataTable<FBodyShape>(DataTables, TEXT("/Game/Data/Hardcoded/BodyShapes.BodyShapes"));
	LoadDataTable<FBodyColor>(DataTables, TEXT("/Game/Data/Hardcoded/BodyColors.BodyColors"));
	LoadDataTable<FHabitat>(DataTables, TEXT("/Game/Data/Hardcoded/Habitats.Habitats"));
	LoadDataTable<FEvolutionData>(DataTables, TEXT("/Game/Data/Hardcoded/Evolutions.Evolutions"));
	LoadDataTable<FStat>(DataTables, TEXT("/Game/Data/Hardcoded/Stats.Stats"));
	LoadDataTable<FNature>(DataTables, TEXT("/Game/Data/Hardcoded/Natures.Natures"));
	LoadDataTable<FStatus>(DataTables, TEXT("/Game/Data/Hardcoded/Statuses.Statuses"));
	LoadDataTable<FWeather>(DataTables, TEXT("/Game/Data/Hardcoded/Weathers.Weathers"));
	LoadDataTable<FEncounterType>(DataTables, TEXT("/Game/Data/Hardcoded/EncounterTypes.EncounterTypes"));
	LoadDataTable<FEnvironment>(DataTables, TEXT("/Game/Data/Hardcoded/Environments.Environments"));
	LoadDataTable<FBattleWeather>(DataTables, TEXT("/Game/Data/Hardcoded/BattleWeathers.BattleWeathers"));
	LoadDataTable<FBattleTerrain>(DataTables, TEXT("/Game/Data/Hardcoded/BattleTerrains.BattleTerrains"));
	LoadDataTable<FTarget>(DataTables, TEXT("/Game/Data/Hardcoded/Targets.Targets"));

	LoadDataTable<FItem>(DataTables, TEXT("/Game/Data/Items.Items"));
}

const IGameData& UDataSubsystem::GetDataTable(TObjectPtr<const UScriptStruct> StructType) const {
	check(DataTables.Contains(StructType));
	return *DataTables[StructType];
}
