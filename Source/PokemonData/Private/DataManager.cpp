// Created by Retro & Chill.


#include "DataManager.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "DataRetrieval/DataRegistry.h"


FDataManager::FDataManager() {
	auto& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	AssetRegistryModule.Get().GetAssetsByClass(FTopLevelAssetPath(UDataTable::StaticClass()->GetPathName()), AssetData);
	for (auto& Iter : AssetData) {
		auto Table = Cast<UDataTable>(Iter.GetAsset());
		if (Table == nullptr)
			continue;

		auto RowStruct = Table->GetRowStruct();
		if (RowStruct == nullptr)
			continue;
		
		auto& DataRegistry = FDataRegistry::GetInstance();
		if (!DataRegistry.IsTypeRegistered(RowStruct))
			continue;

		DataTables.Add(RowStruct->GetFName(), DataRegistry.CreateDataTableProxy(RowStruct, Table));
	}
}

FDataManager::~FDataManager() = default;

FDataManager& FDataManager::GetInstance() {
	static FDataManager DataManager;
	return DataManager;
}

const IGameData& FDataManager::GetDataTable(TObjectPtr<const UScriptStruct> StructType) const {
	check(StructType != nullptr);
	auto StructName = StructType->GetFName();
	check(DataTables.Contains(StructName));
	return *DataTables[StructName];
}
