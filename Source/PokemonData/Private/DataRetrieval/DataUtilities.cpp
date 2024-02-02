// Created by Retro & Chill.


#include "DataRetrieval/DataUtilities.h"

#include "DataSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

void UDataUtilities::GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName,
                             FTableRowBase& OutRow) {
	// We should never hit this!  stubs to avoid NoExport on the class.
	check(false);
}

void UDataUtilities::Generic_GetData(UObject* ContextObject, const UScriptStruct* StructType, FName RowName, void* OutRow) {
	check(StructType != nullptr && OutRow != nullptr);

	auto DataSubsystem = Cast<UDataSubsystem>(USubsystemBlueprintLibrary::GetGameInstanceSubsystem(ContextObject, UDataSubsystem::StaticClass()));
	check(DataSubsystem != nullptr);
	const auto Row = DataSubsystem->GetDataTable(StructType).GetData(RowName);
	StructType->CopyScriptStruct(OutRow, Row);
}