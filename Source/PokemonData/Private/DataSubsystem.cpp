// Created by Retro & Chill.


#include "DataSubsystem.h"

static UDataTable* LoadDataTable(FStringView TableName) {
	ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TableName.GetData());
	check(DataTable.Succeeded());

	return DataTable.Object.Get();
}

UDataSubsystem::UDataSubsystem() {
	GrowthRateTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/GrowthRates.GrowthRates"));
	GenderRatioTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/GenderRatios.GenderRatios"));
	EggGroupTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/EggGroups.EggGroups"));
	BodyShapeTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/BodyShapes.BodyShapes"));
	BodyColorTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/BodyColors.BodyColors"));
	HabitatTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Habitats.Habitats"));
	EvolutionTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Evolutions.Evolutions"));
	StatTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Stats.Stats"));
	NatureTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Natures.Natures"));
	StatusTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Statuses.Statuses"));
	WeatherTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Weathers.Weathers"));
	EncounterTypeTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/EncounterTypes.EncounterTypes"));
	EnvironmentTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Environments.Environments"));
	BattleWeatherTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/BattleWeathers.BattleWeathers"));
	BattleTerrainTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/BattleTerrains.BattleTerrains"));
	TargetTable = LoadDataTable(TEXT("/Game/Data/Hardcoded/Targets.Targets"));

	ItemTable = LoadDataTable(TEXT("/Game/Data/Items.Items"));
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
