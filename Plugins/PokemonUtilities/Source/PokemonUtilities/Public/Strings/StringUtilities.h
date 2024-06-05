// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StringUtilities.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONUTILITIES_API UStringUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", DisplayName = "Starts With a Vowel (Text)")
    static bool StartsWithVowelText(FText Text);

    UFUNCTION(BlueprintPure, Category = "Utilities|Strings", DisplayName = "Starts With a Vowel (String)")
    static bool StartsWithVowelString(const FString& String);

    static bool StartsWithVowel(FStringView String);

};
