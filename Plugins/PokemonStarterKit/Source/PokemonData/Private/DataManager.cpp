// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "DataRetrieval/DataRegistry.h"
#include "PokemonDataSettings.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"

FDataManager::FDataManager() {
    auto Settings = GetDefault<UPokemonDataSettings>();
    // clang-format off
    Settings->DataTables |
        UE::Ranges::Map(&FSoftObjectPath::TryLoad, nullptr) |
        UE::Ranges::CastType<UDataTable> |
        UE::Ranges::ForEach([this](UDataTable *Table) {
            auto RowStruct = Table->GetRowStruct();
            if (RowStruct == nullptr) {
                return;
            }

            const auto &DataRegistry = FDataRegistry::GetInstance();
            if (!DataRegistry.IsTypeRegistered(RowStruct))
                return;

            DataTables.Add(RowStruct->GetFName(), DataRegistry.CreateDataTableProxy(RowStruct, Table));
        });
    // clang-format on
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

bool FDataManager::HasDataTable(const UScriptStruct *StructType) const {
    check(StructType != nullptr)
    auto StructName = StructType->GetFName();
    return DataTables.Contains(StructName);
}

TArray<UScriptStruct *> FDataManager::GetStructTypes() const {
    TArray<UScriptStruct *> Ret;
    for (const auto &[Key, Value] : DataTables) {
        Ret.Add(Value->GetStructType());
    }
    return Ret;
}