// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/DataTableProxy.h"

/**
 * A singleton class used to retrieve the game data when needed
 */
class POKEMONDATA_API FDataManager
{
    FDataManager();
    ~FDataManager();

    FDataManager(const FDataManager &) = delete;
    FDataManager(FDataManager &&) = delete;

    FDataManager &operator=(const FDataManager &) = delete;
    FDataManager &operator=(FDataManager &&) = delete;

  public:
    /**
     * Get the singleton instance of the Data Manager
     * @return A reference to the data manager
     */
    static FDataManager &GetInstance();

    template <typename T>
        requires Pokemon::Data::DataStructHandle<T>
    auto GetData(const T &Handle) const
    {
        return GetDataTable<typename T::FValueType>().GetData(Handle);
    }

    template <typename T>
        requires Pokemon::Data::DataStructHandle<T>
    decltype(auto) GetDataChecked(const T &Handle) const
    {
        return GetDataTable<typename T::FValueType>().GetDataChecked(Handle);
    }

    /**
     * Get the data table that contains data of the specified type
     * @tparam T The type to look up the table for
     * @return A reference to the table proxy object
     */
    template <typename T>
        requires std::is_base_of_v<FTableRowBase, T>
    const TDataTableProxy<T> &GetDataTable() const
    {
        const UScriptStruct *const StructClass = T::StaticStruct();
        auto RowName = StructClass->GetFName();
        check(DataTables.Contains(RowName))

        auto TableInterface = DataTables[RowName].Get();
        check(StructClass->GetName() == TableInterface->GetStructType()->GetName())

        auto TableOut = static_cast<TDataTableProxy<T> *>(TableInterface);
        return *TableOut;
    }

    /**
     * Get the data table that contains data of the specified type
     * @param StructType The type to look up the table for
     * @return A reference to the table proxy object
     */
    const IGameData &GetDataTable(TObjectPtr<const UScriptStruct> StructType) const;

    bool HasDataTable(const UScriptStruct *StructType) const;

    /**
     * Get the set of valid struct types held in this class
     * @return The list of struct types that have tables associated with them
     */
    TArray<UScriptStruct *> GetStructTypes() const;

  private:
    /**
     * The list of data tables in the game
     */
    TMap<FName, TUniquePtr<IGameData>> DataTables;
};