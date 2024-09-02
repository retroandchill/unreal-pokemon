// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "StringUtilities.generated.h"

/**
 * Utility library for operations involving Strings (and Text)
 */
UCLASS()
class POKEMONUTILITIES_API UStringUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
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

    static TSharedPtr<FString> NameToStringPtr(FName Name) {
        return MakeShared<FString>(Name.ToString());
    }
};
