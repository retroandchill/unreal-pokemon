// Created by Retro & Chill.


#include "DataSubsystem.h"

#include "AssetRegistry/AssetRegistryModule.h"
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
#include "DataRetrieval/DataRegistry.h"

UDataSubsystem::UDataSubsystem() {
	auto &AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FTopLevelAssetPath(UDataTable::StaticClass()->GetPathName()), AssetData);
	for (auto &Iter : AssetData) {
		auto Table = Cast<UDataTable>(Iter.GetAsset());
		if (Table == nullptr)
			continue;

		auto RowStruct = Table->GetRowStruct();
		auto &DataRegistry = FDataRegistry::GetInstance();
		if (!DataRegistry.IsTypeRegistered(RowStruct))
			continue;
		
		DataTables.Add(RowStruct->GetFName(), DataRegistry.CreateDataTableProxy(RowStruct, Table));
	}
}

const IGameData& UDataSubsystem::GetDataTable(TObjectPtr<const UScriptStruct> StructType) const {
	check(StructType != nullptr);
	auto StructName = StructType->GetFName();
	check(DataTables.Contains(StructName));
	return *DataTables[StructName];
}
