// "Unreal Pokémon" created by Retro & Chill.
#include "DataManager.h"
#include "DataRetrieval/DataRegistry.h"
#include "PokemonDataSettings.h"
#include "Ranges/Views/ContainerView.h"
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/transform.hpp>

FDataManager::FDataManager() {
    auto Settings = GetDefault<UPokemonDataSettings>();
    auto DataTableView = UE::Ranges::CreateRange(Settings->DataTables) |
                         ranges::views::transform([](const FSoftObjectPath &Path) { return Path.TryLoad(); }) |
                         ranges::views::transform([](UObject *Object) { return CastChecked<UDataTable>(Object); });
    ranges::for_each(DataTableView, [this](UDataTable *Table) {
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
