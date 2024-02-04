// Created by Retro & Chill.


#include "DataRetrieval/DataRegistry.h"

FDataRegistry::FDataRegistry() = default;

FDataRegistry::~FDataRegistry() = default;

FDataRegistry& FDataRegistry::GetInstance() {
	static FDataRegistry DataRegistry;
	return DataRegistry;
}

TUniquePtr<IGameData> FDataRegistry::CreateDataTableProxy(UScriptStruct* StructType, const TObjectPtr<UDataTable>& DataTable) const {
	check(StructType != nullptr);
	auto TypeName = StructType->GetFName();

	check(RegisteredConstructors.Contains(TypeName));
	return RegisteredConstructors[TypeName](DataTable);
}
