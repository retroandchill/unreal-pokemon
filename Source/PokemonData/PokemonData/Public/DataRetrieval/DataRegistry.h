// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "DataRetrieval/GameData.h"
#include "DataRetrieval/DataTableProxy.h"
#include "Meta/PokeRegistry.h"
#include "Meta/UniquePokeRegistry.h"

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
	using FFactoryFunction = TFunction<TUniquePtr<IGameData>(UDataTable*)>;

	/**
	 * The internal registry used to actually retrieve the objects.
	 * @return The registry in question to retrieve
	 */
	using FInternalRegistry = TUniquePokeRegistry<IGameData, UDataTable*>;

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
		const UScriptStruct* const TypeStruct = T::StaticStruct();
		Registry.RegisterClass<TDataTableProxy<T>>(TypeStruct->GetFName());
		return true;
	}

	/**
	 * Create a data table proxy of the specified type
	 * @param StructType The type that the proxy references
	 * @param DataTable The data table asset this proxy points to
	 * @return A unique reference to the proxy
	 */
	TUniquePtr<IGameData> CreateDataTableProxy(const UScriptStruct* StructType,
	                                           const TObjectPtr<UDataTable>& DataTable) const;

	/**
	 * Get if the type in question is registered in as a valid constructor
	 * @param StructType The struct type in question
	 * @return Is the type registered?
	 */
	bool IsTypeRegistered(const UScriptStruct* StructType) const;

private:
	/**
	 * The internal registry type that handles this particular registration
	 */
	FInternalRegistry Registry;
};
