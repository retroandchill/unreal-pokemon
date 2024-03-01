// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "GameData.h"
#include "Memory/RootMemoryPointers.h"

/**
 * Proxy class that stores a data table and allows the retrieval of properties from it
 */
template <typename T>
class POKEMONDATA_API TDataTableProxy final : public IGameData {
public:
	explicit TDataTableProxy(UDataTable* DataTable) : DataTable(MakeUniqueRoot(DataTable)) {
	}

	UStruct* GetStructType() const override {
		return T::StaticStruct();
	}

	T* GetData(FName ID) const override {
		return DataTable->FindRow<T>(ID, "Failed to find the specified data!");
	}

	TSet<FName> GetTableRowNames() const override {
		return TSet(DataTable->GetRowNames());
	}

	bool IsRowNameValid(FName ID) const override {
		return GetTableRowNames().Contains(ID);
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
