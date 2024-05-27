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

    T *GetData(FName ID) const override {
        return DataTable->FindRow<T>(ID, TEXT("Find row!"));
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
    void ForEach(TFunctionRef<void(const T &)> Callback) const {
        for (auto Rows = GetAllRows(); auto Row : Rows) {
            const T &Ref = *Row;
            Callback(Ref);
        }
    }

    TArray<const T *> Filter(TFunctionRef<bool(const T &)> Predicate) const {
        TArray<const T *> Rows;
        ForEach([&Rows, &Predicate](const T &Row) {
            if (Predicate(Row)) {
                Rows.Add(&Row);
            }
        });

        return Rows;
    }

    template <typename R>
    TArray<R> Map(TFunctionRef<R(const T &)> Mapping) const {
        TArray<R> Ret;
        ForEach([&Ret, &Mapping](const T &Row) { Ret.Add(Mapping(Row)); });
        return Ret;
    }

  private:
    /**
     * A pointer to the data table asset that this proxy object contains
     */
    TUniqueRootPtr<UDataTable> DataTable;
};
