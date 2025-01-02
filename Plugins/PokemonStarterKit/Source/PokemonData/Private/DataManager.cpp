// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "DataRetrieval/DataRegistry.h"
#include "PokemonDataSettings.h"


FDataManager::FDataManager() {
    auto Settings = GetDefault<UPokemonDataSettings>();
    // clang-format off
    Settings->DataTables |
        Retro::Ranges::Views::Transform<&FSoftObjectPath::TryLoad>(nullptr) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<UDataTable>) |
        Retro::Ranges::ForEach([this](UDataTable *Table) {
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