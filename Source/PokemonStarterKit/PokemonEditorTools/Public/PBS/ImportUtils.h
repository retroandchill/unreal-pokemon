// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ImportUtils.generated.h"

/**
 * Utility functions to handle importing data from data tables. Mainly used for the Python API
 */
UCLASS(BlueprintType)
class POKEMONEDITORTOOLS_API UImportUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Select a file from the filesystem and set it to a text box
     * @param FileTypes The filter for the filetypes to employ in the selector
     * @return The full qualified path of the filename,
     */
    UFUNCTION(BlueprintCallable, Category = "Editor|Filesystem")
    static FText SelectFile(const FString &FileTypes);

    UFUNCTION(meta = (ScriptMethod))
    static bool TryAddGameplayTagToIni(FName TagSource, const FString &TagName, FString &Error);
};
