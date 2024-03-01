// "Unreal Pokémon" created by Retro & Chill.
#include "DataRetrieval/DataUtilities.h"

void UDataUtilities::GetData(const UScriptStruct* StructType, FName RowName,
                             FTableRowBase& OutRow) {
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(false);
}

TSet<FName> UDataUtilities::GetAllDataIDs(const UScriptStruct* StructType) {
	return FDataManager::GetInstance().GetDataTable(StructType).GetTableRowNames();
}

bool UDataUtilities::IsDataRowNameValid(UObject* ContextObject, const UScriptStruct* StructType, FName RowName) {
	return FDataManager::GetInstance().GetDataTable(StructType).IsRowNameValid(RowName);
}
