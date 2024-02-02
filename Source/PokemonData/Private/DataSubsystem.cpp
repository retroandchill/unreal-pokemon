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
static void LoadDataTable(TMap<TObjectPtr<UStruct>, TUniquePtr<IGameData>>& TableMap, FStringView TableName) {
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

template <typename T>
static const T &GetData(UDataTable *Table, FName ID) {
	auto Entry = Table->FindRow<T>(ID, "Failed to find the specified data!");
	check(Entry != nullptr);
	return *Entry;
}

const FGrowthRateData& UDataSubsystem::GetGrowthRate(FName ID) const {
	return GetData<FGrowthRateData>(GrowthRateTable, ID);
}

TArray<FName> UDataSubsystem::GetGrowthRateIds() const {
	return GrowthRateTable->GetRowNames();
}

bool UDataSubsystem::IsGrowthRateIdValid(FName ID) const {
	return GetGrowthRateIds().Contains(ID);
}

const FGenderRatio& UDataSubsystem::GetGenderRatio(FName ID) const {
	return GetData<FGenderRatio>(GenderRatioTable, ID);
}

TArray<FName> UDataSubsystem::GetGenderRatioIds() const {
	return GenderRatioTable->GetRowNames();
}

bool UDataSubsystem::IsGenderRatioIdValid(FName ID) const {
	return GetGenderRatioIds().Contains(ID);
}

const FEggGroup& UDataSubsystem::GetEggGroup(FName ID) const {
	return GetData<FEggGroup>(EggGroupTable, ID);
}

TArray<FName> UDataSubsystem::GetEggGroupIds() const {
	return EggGroupTable->GetRowNames();
}

bool UDataSubsystem::IsEggGroupIdValid(FName ID) const {
	return GetEggGroupIds().Contains(ID);
}

const FBodyShape& UDataSubsystem::GetBodyShape(FName ID) const {
	return GetData<FBodyShape>(BodyShapeTable, ID);
}

TArray<FName> UDataSubsystem::GetBodyShapeIds() const {
	return BodyShapeTable->GetRowNames();
}

bool UDataSubsystem::IsBodyShapeIdValid(FName ID) const {
	return GetBodyShapeIds().Contains(ID);
}

const FBodyColor& UDataSubsystem::GetBodyColor(FName ID) const {
	return GetData<FBodyColor>(BodyColorTable, ID);
}

TArray<FName> UDataSubsystem::GetBodyColorIds() const {
	return BodyColorTable->GetRowNames();
}

bool UDataSubsystem::IsBodyColorIdValid(FName ID) const {
	return GetBodyColorIds().Contains(ID);
}

const FHabitat& UDataSubsystem::GetHabitat(FName ID) const {
	return GetData<FHabitat>(HabitatTable, ID);
}

TArray<FName> UDataSubsystem::GetHabitatIds() const {
	return HabitatTable->GetRowNames();
}

bool UDataSubsystem::IsHabitatIdValid(FName ID) const {
	return GetHabitatIds().Contains(ID);
}

const FEvolutionData& UDataSubsystem::GetEvolution(FName ID) const {
	return GetData<FEvolutionData>(EvolutionTable, ID);
}

TArray<FName> UDataSubsystem::GetEvolutionIds() const {
	return EvolutionTable->GetRowNames();
}

bool UDataSubsystem::IsEvolutionIdValid(FName ID) const {
	return GetEvolutionIds().Contains(ID);
}

const FStat& UDataSubsystem::GetStat(FName ID) const {
	return GetData<FStat>(StatTable, ID);
}

TArray<FName> UDataSubsystem::GetStatIds() const {
	return StatTable->GetRowNames();
}

bool UDataSubsystem::IsStatIdValid(FName ID) const {
	return GetStatIds().Contains(ID);
}

const FNature& UDataSubsystem::GetNature(FName ID) const {
	return GetData<FNature>(NatureTable, ID);
}

TArray<FName> UDataSubsystem::GetNatureIds() const {
	return NatureTable->GetRowNames();
}

bool UDataSubsystem::IsNatureIdValid(FName ID) const {
	return GetNatureIds().Contains(ID);
}

const FStatus& UDataSubsystem::GetStatus(FName ID) const {
	return GetData<FStatus>(StatusTable, ID);
}

TArray<FName> UDataSubsystem::GetStatusIds() const {
	return StatusTable->GetRowNames();
}

bool UDataSubsystem::IsStatusIdValid(FName ID) const {
	return GetStatusIds().Contains(ID);
}

const FWeather& UDataSubsystem::GetWeather(FName ID) const {
	return GetData<FWeather>(WeatherTable, ID);
}

TArray<FName> UDataSubsystem::GetWeatherIds() const {
	return WeatherTable->GetRowNames();
}

bool UDataSubsystem::IsWeatherIdValid(FName ID) const {
	return GetWeatherIds().Contains(ID);
}

const FEncounterType& UDataSubsystem::GetEncounterType(FName ID) const {
	return GetData<FEncounterType>(EncounterTypeTable, ID);
}

TArray<FName> UDataSubsystem::GetEncounterTypeIds() const {
	return EncounterTypeTable->GetRowNames();
}

bool UDataSubsystem::IsEncounterTypeIdValid(FName ID) const {
	return GetEncounterTypeIds().Contains(ID);
}

const FEnvironment& UDataSubsystem::GetEnvironment(FName ID) const {
	return GetData<FEnvironment>(EnvironmentTable, ID);
}

TArray<FName> UDataSubsystem::GetEnvironmentIds() const {
	return EnvironmentTable->GetRowNames();
}

bool UDataSubsystem::IsEnvironmentIdValid(FName ID) const {
	return GetEnvironmentIds().Contains(ID);
}

const FBattleWeather& UDataSubsystem::GetBattleWeather(FName ID) const {
	return GetData<FBattleWeather>(BattleWeatherTable, ID);
}

TArray<FName> UDataSubsystem::GetBattleWeatherIds() const {
	return BattleWeatherTable->GetRowNames();
}

bool UDataSubsystem::IsBattleWeatherIdValid(FName ID) const {
	return GetBodyColorIds().Contains(ID);
}

const FBattleTerrain& UDataSubsystem::GetBattleTerrain(FName ID) const {
	return GetData<FBattleTerrain>(BattleTerrainTable, ID);
}

TArray<FName> UDataSubsystem::GetBattleTerrainIds() const {
	return BattleTerrainTable->GetRowNames();
}

bool UDataSubsystem::IsBattleTerrainIdValid(FName ID) const {
	return GetBattleTerrainIds().Contains(ID);
}

const FTarget& UDataSubsystem::GetTarget(FName ID) const {
	return GetData<FTarget>(TargetTable, ID);
}

TArray<FName> UDataSubsystem::GetTargetIds() const {
	return TargetTable->GetRowNames();
}

bool UDataSubsystem::IsTargetIdValid(FName ID) const {
	return GetTargetIds().Contains(ID);
}

const FItem& UDataSubsystem::GetItem(FName ID) const {
	return GetData<FItem>(ItemTable, ID);
}

TArray<FName> UDataSubsystem::GetItemIds() const {
	return ItemTable->GetRowNames();
}

bool UDataSubsystem::IsItemIdValid(FName ID) const {
	return GetItemIds().Contains(ID);
}

const IGameData& UDataSubsystem::GetDataTable(TObjectPtr<UStruct> StructType) {
	check(DataTables.Contains(StructType));
	return *DataTables[StructType];
}
