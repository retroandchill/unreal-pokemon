// "Unreal Pokémon" created by Retro & Chill.

#include "Strings/StringUtilities.h"

bool UStringUtilities::StartsWithVowelText(FText Text) {
    return StartsWithVowel(Text.ToString());
}

bool UStringUtilities::StartsWithVowelString(const FString &String) {
    return StartsWithVowel(String);
}

bool UStringUtilities::StartsWithVowel(FStringView String) {
    if (String.IsEmpty()) {
        return false;
    }

    static const TArray<TCHAR, TInlineAllocator<5>> Vowels = {'a', 'e', 'i', 'o', 'u'};
    return Vowels.Contains(FChar::ToLower(String[0]));
}