// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Species/Stat.h"
#include "ImportUtils.generated.h"

/**
 * Utility functions to handle importing data from data tables. Mainly used for the Python API
 */
UCLASS(BlueprintType)
class POKEMONEDITORUTILS_API UImportUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	/**
	 * Get the list of Stat elements from the table that exclude any battle stats
	 * @param DataTable The data table to draw from
	 * @return The list of main and main/battle stats
	 */
	UFUNCTION(BlueprintPure, Category = "Editor|Data")
	static TArray<FStat> GetMainStatsFromTable(UDataTable *DataTable);
};
