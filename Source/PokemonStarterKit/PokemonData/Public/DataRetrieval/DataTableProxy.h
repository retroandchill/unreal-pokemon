// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DataStructHandle.h"
#include "GameData.h"
#include "RetroLib/Casting/ByteCast.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 * @tparam T The row type this proxy references
 */
template <typename T>
    requires Pokemon::Data::DataStruct<T>
class TDataTableProxy final : public IGameData
{
  public:
    explicit TDataTableProxy(UDataTable *DataTable) : DataTable(DataTable)
    {
    }

    UScriptStruct *GetStructType() const override
    {
        return T::StaticStruct();
    }

    const T *GetData(FName ID) const override
    {
        return DataTable->FindRow<T>(ID, TEXT("Find row!"));
    }

    const T &GetDataChecked(FName ID) const override
    {
        auto Data = DataTable->FindRow<T>(ID, TEXT("Find row!"));
        check(Data != nullptr)
        return *Data;
    }

    TArray<FName> GetTableRowNames() const override
    {
        return DataTable->GetRowNames();
    }

    bool IsRowNameValid(FName ID) const override
    {
        return GetTableRowNames().Contains(ID);
    }

    UDataTable *GetDataTable() const override
    {
        return DataTable.Get();
    }

    auto GetAllRows() const
    {
        // clang-format off
        return DataTable->GetRowMap() |
               Retro::Ranges::Views::Values |
               Retro::Ranges::Views::Transform(Retro::ByteCast<T>);
        // clang-format on
    }

  private:
    /**
     * A pointer to the data table asset that this proxy object contains
     */
    TStrongObjectPtr<UDataTable> DataTable;
};