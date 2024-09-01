// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "IndexedTableRow.h"
#include "Memory/RootMemoryPointers.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 * @tparam T The row type this proxy references
 */
template <typename T>
    requires std::is_base_of_v<FIndexedTableRow, T>
class TDataTableProxy final : public IGameData {
  public:
    explicit TDataTableProxy(UDataTable *DataTable) : DataTable(MakeUniqueRoot(DataTable)) {
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

    TArray<T *> GetAllRows() const {
        TArray<T *> Rows;
        DataTable->GetAllRows(TEXT("ForEach"), Rows);
        return Rows;
    }

    /**
     * Iterate through the data table's rows and execute the callback on each entry
     * @param Callback The callback method
     */
    template <typename F>
        requires std::invocable<F, const T&>
    void ForEach(F&& Callback) const {
        for (auto Rows = GetAllRows(); auto Row : Rows) {
            const T &Ref = *Row;
            Callback(Ref);
        }
    }

  private:
    /**
     * A pointer to the data table asset that this proxy object contains
     */
    TUniqueRootPtr<UDataTable> DataTable;
};
