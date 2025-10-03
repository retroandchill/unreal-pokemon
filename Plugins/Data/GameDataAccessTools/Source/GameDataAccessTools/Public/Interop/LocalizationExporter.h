// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "LocalizationExporter.generated.h"

/**
 * @class ULocalizationExporter
 * @brief Utility class for handling localized text generation and parsing.
 *
 * This class provides static functions for creating localized text entries
 * and parsing strings into localized text.
 */
UCLASS()
class GAMEDATAACCESSTOOLS_API ULocalizationExporter : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * @brief Creates a localized text entry using the specified namespace, key, and default value.
     *
     * This function generates a localized text object based on the provided namespace, key,
     * and default value, and assigns it to the output parameter.
     *
     * @param Namespace The namespace to categorize the localized text entry.
     * @param Key The key to uniquely identify the localized text entry within the namespace.
     * @param DefaultValue The default value to use if no localized entry is found.
     * @param OutText The output parameter that will contain the generated localized text.
     */
    UNREALSHARP_FUNCTION()
    static void CreateLocalizedText(const TCHAR *Namespace, const TCHAR *Key, const TCHAR *DefaultValue,
                                    FText &OutText);

    /**
     * @brief Parses a localized string into an FText object.
     *
     * This function attempts to read a localized text from the provided string buffer.
     * If the parsing fails, it falls back to creating a simple FText object using the input string.
     *
     * @param LocalizedString The input string to parse as localized text.
     * @param OutText The output parameter that will contain the resulting FText object.
     */
    UNREALSHARP_FUNCTION()
    static void FromLocalizedString(const TCHAR *LocalizedString, FText &OutText);
};
