// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "ImportUtils.generated.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Species/Stat.h"

/**
 * Utility functions to handle importing data from data tables. Mainly used for the Python API
 */
UCLASS(BlueprintType)
class POKEMONEDITORUTILS_API UImportUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of Stat elements from the table that exclude any battle stats
     * @param DataTable The data table to draw from
     * @return The list of main and main/battle stats
     */
    UFUNCTION(BlueprintPure, Category = "Editor|Data")
    static TArray<FStat> GetMainStatsFromTable(const UDataTable *const DataTable);

    /**
     * Select a file from the filesystem and set it to a text box
     * @param FileTypes The filter for the filetypes to employ in the selector
     * @return The full qualified path of the filename,
     */
    UFUNCTION(BlueprintCallable, Category = "Editor|Filesystem")
    static FText SelectFile(const FString &FileTypes);
};
