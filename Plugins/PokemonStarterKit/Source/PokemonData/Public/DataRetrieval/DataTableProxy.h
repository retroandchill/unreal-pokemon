// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "IndexedTableRow.h"
#include "Ranges/Concepts/Structs.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/MapValue.h"

namespace Pokemon::Data {
    /**
     * Concept for a struct that is used for a data proxy.
     * @tparam T The type to verify the struct status against
     */
    template <typename T>
    concept DataStruct = UE::Ranges::UEStruct<T> && std::is_base_of_v<FIndexedTableRow, T>;

    /**
     * The struct for
     */
    template <typename T>
    concept DataStructHandle = DataStruct<typename std::remove_cvref_t<T>::FValueType> &&
        std::convertible_to<FName, std::remove_reference_t<T>> && std::convertible_to<T, FName>;
}

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 * @tparam T The row type this proxy references
 */
template <typename T>
    requires Pokemon::Data::DataStruct<T>
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
        // clang-format off
        return DataTable->GetRowMap() |
               UE::Ranges::MapValue |
               UE::Ranges::CastType<T>;
        // clang-format on
    }

private:
    /**
     * A pointer to the data table asset that this proxy object contains
     */
    TStrongObjectPtr<UDataTable> DataTable;
};