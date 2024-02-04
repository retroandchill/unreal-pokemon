// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/GameData.h"
#include "DataRetrieval/DataTableProxy.h"

/**
 * Internal registry used to handle the tracking of the data table structs
 */
class POKEMONDATA_API FDataRegistry {
	FDataRegistry();
	~FDataRegistry();

public:
	/**
	 * Factory function that produces a unique pointer to the proxy type
	 */
	using FFactoryFunction = TFunction<TUniquePtr<IGameData>(const TObjectPtr<UDataTable>& DataTable)>;

	/**
	 * Get the singleton instance of the class
	 * @return A reference to the only instance of this class
	 */
	static FDataRegistry& GetInstance();

	/**
	 * Register the constructor of the given type
	 * @tparam T The type that the proxy references
	 * @return The registration succeeded
	 */
	template <typename T>
	bool RegisterStruct() {
		UScriptStruct *TypeStruct = T::StaticStruct();
		RegisteredConstructors.Add(TypeStruct->GetFName(), CreateDataTableProxy<T>);
		return true;
	}

	/**
	 * Create a data table proxy of the specified type
	 * @param StructType The type that the proxy references
	 * @param DataTable The data table asset this proxy points to
	 * @return A unique reference to the proxy
	 */
	TUniquePtr<IGameData> CreateDataTableProxy(UScriptStruct *StructType, const TObjectPtr<UDataTable>& DataTable) const;
	

private:
	/**
	 * Create a data table proxy of the specified type
	 * @tparam T The type that the proxy references
	 * @return A unique reference to the proxy
	 */
	template <typename T>
	static TUniquePtr<IGameData> CreateDataTableProxy(const TObjectPtr<UDataTable>& DataTable) {
		return MakeUnique<TDataTableProxy<T>>(DataTable);
	}

	/**
	 * The set of registered constructors for the data table proxy types
	 */
	TMap<FName, FFactoryFunction> RegisteredConstructors;
};

#define REGISTER_DATA_TABLE(StructType) const bool GRegistered_##StructType = FDataRegistry::GetInstance().RegisterStruct<StructType>()