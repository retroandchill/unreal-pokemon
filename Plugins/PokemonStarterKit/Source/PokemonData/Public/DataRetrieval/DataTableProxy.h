// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "IndexedTableRow.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/MapValue.h"
#include "Ranges/Views/ContainerView.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 * @tparam T The row type this proxy references
 */
template <typename T>
    requires std::is_base_of_v<FIndexedTableRow, T>
class TDataTableProxy final : public IGameData {
  public:
    explicit TDataTableProxy(UDataTable *DataTable) : DataTable(DataTable) {
    }

    UScriptStruct *GetStructType() const override {
        return T::StaticStruct();
    }

    const T *GetData(FName ID) const override {
        return DataTable->FindRow<T>(ID, TEXT("Find row!"));
    }

    const T &GetDataChecked(FName ID) const override {
        auto Data = DataTable->FindRow<T>(ID, TEXT("Find row!"));
        check(Data != nullptr)
        return *Data;
    }

    TArray<FName> GetTableRowNames() const override {
        return DataTable->GetRowNames();
    }

    bool IsRowNameValid(FName ID) const override {
        return GetTableRowNames().Contains(ID);
    }

    UDataTable *GetDataTable() const override {
        return DataTable.Get();
    }

    auto GetAllRows() const {
        return DataTable->GetRowMap() |
            UE::Ranges::MapValue |
            UE::Ranges::CastType<T>;
    }

  private:
    /**
     * A pointer to the data table asset that this proxy object contains
     */
    TStrongObjectPtr<UDataTable> DataTable;
};
