// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "Memory/RootMemoryPointers.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 * @tparam T The row type this proxy references
 */
template <typename T>
requires std::is_base_of_v<FTableRowBase, T>
class POKEMONDATA_API TDataTableProxy final : public IGameData {
public:
	explicit TDataTableProxy(UDataTable* DataTable) : DataTable(MakeUniqueRoot(DataTable)) {
	}

	UScriptStruct* GetStructType() const override {
		return T::StaticStruct();
	}

	T* GetData(FName ID) const override {
		return GetDataManaged(ID).Get();
	}

	/**
	 * Get a managed version of the stored data
	 * @param ID The managed data
	 * @return A managed pointer to the data
	 */
	TRowPointer<T> GetDataManaged(FName ID) const {
		return TRowPointer<T>(*DataTable, ID);
	}

	TArray<FName> GetTableRowNames() const override {
		return DataTable->GetRowNames();
	}

	bool IsRowNameValid(FName ID) const override {
		return GetTableRowNames().Contains(ID);
	}

	UDataTable* GetDataTable() const override {
		return DataTable.Get();
	}

	/**
	 * Iterate through the data table's rows and execute the callback on each entry
	 * @tparam Functor The signature of the callback
	 * @param Callback The callback method
	 */
	template <typename Functor>
	void ForEach(Functor Callback) const {
		TArray<T*> Rows;
		DataTable->GetAllRows(TEXT("ForEach"), Rows);
		for (auto Row : Rows) {
			const T& Ref = *Row;
			Callback(Ref);
		}
	}

private:
	/**
	 * A pointer to the data table asset that this proxy object contains
	 */
	TUniqueRootPtr<UDataTable> DataTable;
};
