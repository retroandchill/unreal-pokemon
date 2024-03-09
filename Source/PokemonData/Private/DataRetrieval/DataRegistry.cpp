// "Unreal Pokémon" created by Retro & Chill.
#include "DataRetrieval/DataRegistry.h"

#include "Meta/PokeRegistry.h"

FDataRegistry::FDataRegistry() = default;

FDataRegistry::~FDataRegistry() = default;

FDataRegistry& FDataRegistry::GetInstance() {
	static FDataRegistry DataRegistry;
	return DataRegistry;
}

TUniquePtr<IGameData> FDataRegistry::CreateDataTableProxy(const UScriptStruct* StructType,
                                                          const TObjectPtr<UDataTable>& DataTable) const {
	check(StructType != nullptr)
	return Registry.Construct(StructType->GetFName(), DataTable);
}

bool FDataRegistry::IsTypeRegistered(const UScriptStruct* StructType) const {
	check(StructType != nullptr)
	return Registry.IsTypeRegistered(StructType->GetFName());
}
