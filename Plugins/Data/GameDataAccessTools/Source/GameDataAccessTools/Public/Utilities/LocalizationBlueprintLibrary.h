// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "LocalizationBlueprintLibrary.generated.h"

/**
 * ULocalizationBlueprintLibrary
 *
 * A utility class to provide functionality for working with text localization in Unreal Engine.
 * This class extends UBlueprintFunctionLibrary and provides static methods for text manipulation
 * and inspection in the context of localization.
 */
UCLASS()
class GAMEDATAACCESSTOOLS_API ULocalizationBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Converts the given localized FText to a plain FString representation.
     *
     * This method allows retrieving the internal string value of a localized FText object.
     * It is useful when working with text in a more straightforward non-localized string format.
     *
     * @param Text The source FText to be converted into a plain FString.
     * @return A FString representation of the input FText.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FString ToLocalizedString(const FText &Text);

    /**
     * Attempts to retrieve the namespace associated with a given localized FText.
     *
     * This method inspects the provided FText to determine if it has an associated namespace in its metadata.
     * If a namespace exists, it is returned via the OutNamespace parameter.
     *
     * @param Text The source FText to inspect for an associated namespace.
     * @param OutNamespace A reference to an FString that will hold the namespace if it exists. If no namespace is
     * found, this will be set to an empty string.
     * @return True if the FText has an associated namespace and it was successfully retrieved; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool TryGetNamespace(const FText &Text, FString &OutNamespace);

    /**
     * Attempts to retrieve the key associated with a given localized FText.
     *
     * This method inspects the provided FText to determine if it has an associated key in its metadata.
     * If a key exists, it is returned via the OutKey parameter.
     *
     * @param Text The source FText to inspect for an associated key.
     * @param OutKey A reference to an FString that will hold the key if it exists. If no key is found, this will be set
     * to an empty string.
     * @return True if the FText has an associated key and it was successfully retrieved; otherwise, false.
     */
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool TryGetKey(const FText &Text, FString &OutKey);
};
