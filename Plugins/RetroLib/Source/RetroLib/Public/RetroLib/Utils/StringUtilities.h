// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/JoinWith.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

#include "StringUtilities.generated.h"

/**
 * Utility library for operations involving Strings (and Text)
 */
UCLASS()
class RETROLIB_API UStringUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    static const FText ConjunctionAnd;
    static const FText ConjunctionOr;
    
    /**
     * Check if the supplied text starts with a vowel
     * @param Text The supplied text
     * @return does it start with a vowel
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", DisplayName = "Starts With a Vowel (Text)",
              meta = (AutoCreateRefTerm = Text))
    static bool StartsWithVowelText(const FText &Text);

    /**
     * Check if the supplied text starts with a vowel
     * @param String The supplied text
     * @return does it start with a vowel
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", DisplayName = "Starts With a Vowel (String)")
    static bool StartsWithVowelString(const FString &String);

    /**
     * Check if the supplied text starts with a vowel
     * @param String The supplied text
     * @return does it start with a vowel
     */
    static bool StartsWithVowel(FStringView String);

    /**
     * Take a list of strings and generate a single text string list
     * @param Items The items in question to concatenate
     * @param Conjunction The contraction to use on the last two items
     * @param bOxfordComma Should an oxford comma be applied to a list of 3 or more?
     * @return The generated list string
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", meta = (AutoCreateRefTerm = Conjunction))
    static FText GenerateList(const TArray<FText> &Items, const FText &Conjunction, bool bOxfordComma = true);

    /**
     * Converts an FName object to a shared pointer of FString.
     *
     * @param Name The FName object to be converted.
     * @return A shared pointer to the resulting FString.
     */
    static TSharedPtr<FString> NameToStringPtr(FName Name);

    /**
     * Formats a given DateTime object into a string based on the specified format.
     *
     * @param DateTime The FDateTime object to be formatted.
     * @param Format The format string to use for formatting the DateTime. Default is "%b %e, %Y".
     * @return The formatted date as a string.
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", meta = (AutoCreateRefTerm = "DateTime,Format"))
    static FString FormatDate(const FDateTime &DateTime, const FString &Format = TEXT("%b %e, %Y"));

    /**
     * Joins an array of FText lines into a single FText separated by newline characters.
     *
     * @param Lines The array of FText lines to join.
     * @return A single FText containing all the lines joined with newline characters.
     */
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", meta = (AutoCreateRefTerm = Delimiter))
    static FText JoinText(const TArray<FText> &Lines);

    template <std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, FText>
    static FText JoinText(R&& Lines) {
        // clang-format off
        return FText::FromString(Lines |
                                 Retro::Ranges::Views::Transform(&FText::ToString) |
                                 Retro::Ranges::Views::JoinWith(TEXT("\n")) |
                                 Retro::Ranges::To<FString>());
        // clang-format on
    }
};
