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

    /**
     * Attempts to add a new gameplay tag to the INI files. If the tag is valid and properly added,
     * the operation succeeds. Otherwise, an error message is provided.
     *
     * @param TagSource The name of the source (e.g., file or module) where the tag is being added.
     * @param TagName The name of the gameplay tag to add.
     * @param Error Outputs any error message that occurs if the tag is not valid or the operation fails.
     * @return True if the gameplay tag was successfully added, otherwise false.
     */
    UFUNCTION(meta = (ScriptMethod))
    static bool TryAddGameplayTagToIni(FName TagSource, const FString &TagName, FString &Error);
};
