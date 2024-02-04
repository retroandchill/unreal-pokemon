// Created by Retro & Chill.


#include "DataRetrieval/DataUtilities.h"

void UDataUtilities::GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName,
                             FTableRowBase& OutRow) {
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(false);
}

TSet<FName> UDataUtilities::GetAllDataIDs(UObject* ContextObject, const UScriptStruct* StructType) {
	const auto DataSubsystem = Cast<UDataSubsystem>(
			USubsystemBlueprintLibrary::GetGameInstanceSubsystem(ContextObject, UDataSubsystem::StaticClass()));
	check(DataSubsystem != nullptr);
	return DataSubsystem->GetDataTable(StructType).GetTableRowNames();
}
