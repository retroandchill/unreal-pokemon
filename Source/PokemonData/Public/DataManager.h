// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/DataTableProxy.h"

/**
 * A singleton class used to retrieve the game data when needed
 */
class POKEMONDATA_API FDataManager {
	FDataManager();
	~FDataManager();

	FDataManager(const FDataManager &) = delete;
	FDataManager(FDataManager &&) = delete;

	FDataManager &operator=(const FDataManager&) = delete;
	FDataManager &operator=(FDataManager&&) = delete;

public:
	/**
	 * Get the singleton instance of the Data Manager
	 * @return A reference to the data manager
	 */
	static FDataManager &GetInstance();
	
	/**
	 * Get the data table that contains data of the specified type
	 * @tparam T The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	template <typename T>
	const TDataTableProxy<T>& GetDataTable() const {
		UScriptStruct* StructClass = T::StaticStruct();
		auto RowName = StructClass->GetFName();
		check(DataTables.Contains(RowName));

		auto TableInterface = DataTables[RowName].Get();
		check(StructClass->GetName() == TableInterface->GetStructType()->GetName())

		auto TableOut = static_cast<TDataTableProxy<T>*>(TableInterface);
		return *TableOut;
	}

	/**
	 * Get the data table that contains data of the specified type
	 * @param StructType The type to look up the table for
	 * @return A reference to the table proxy object
	 */
	const IGameData& GetDataTable(TObjectPtr<const UScriptStruct> StructType) const;

private:
	/**
	 * The list of data tables in the game
	 */
	TMap<FName, TUniquePtr<IGameData>> DataTables;
};
