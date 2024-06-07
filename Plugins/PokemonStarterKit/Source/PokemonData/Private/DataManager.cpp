// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "DataRetrieval/DataRegistry.h"
#include "Settings/BaseSettings.h"

FDataManager::FDataManager() {
    Pokemon::FBaseSettings::Get().ForEachDataTable([this](UDataTable* Table) {
        auto RowStruct = Table->GetRowStruct();
         if (RowStruct == nullptr) {
             return;
         }

         const auto &DataRegistry = FDataRegistry::GetInstance();
         if (!DataRegistry.IsTypeRegistered(RowStruct))
             return;

         DataTables.Add(RowStruct->GetFName(), DataRegistry.CreateDataTableProxy(RowStruct, Table)); 
    });
}

FDataManager::~FDataManager() = default;

FDataManager &FDataManager::GetInstance() {
    static FDataManager DataManager;
    return DataManager;
}

const IGameData &FDataManager::GetDataTable(TObjectPtr<const UScriptStruct> StructType) const {
    check(StructType != nullptr)
    auto StructName = StructType->GetFName();
    check(DataTables.Contains(StructName))
    return *DataTables[StructName];
}

TArray<UScriptStruct *> FDataManager::GetStructTypes() const {
    TArray<UScriptStruct *> Ret;
    for (const auto &[Key, Value] : DataTables) {
        Ret.Add(Value->GetStructType());
    }
    return Ret;
}
