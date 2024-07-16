// "Unreal Pokémon" created by Retro & Chill.

#include "Strings/StringUtilities.h"
#include "RangeHelpers.h"
#include <range/v3/view/transform.hpp>

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

FText UStringUtilities::GenerateList(const TArray<FText> &Items, FText Contraction, bool bOxfordComma) {
    if (Items.IsEmpty()) {
        return FText::GetEmpty();
    }

    auto ExtractString = [](const FText &Text) -> const FString & { return Text.ToString(); };

    if (Items.Num() <= 2) {
        auto StringItems = RangeHelpers::CreateRange(Items) | ranges::views::transform(ExtractString);
        return FText::FromString(FString::Join(StringItems, *Contraction.ToString()));
    }

    auto CommaSeparatedItems = ranges::span(Items.GetData(), Items.Num() - 1) | ranges::views::transform(ExtractString);
    auto JoinedItems = FString::Join(CommaSeparatedItems, TEXT(", "));
    if (bOxfordComma) {
        JoinedItems.Append(TEXT(", "));
    }

    JoinedItems.Appendf(TEXT(" %s %s"), *Contraction.ToString(), *Items.Last().ToString());
    return FText::FromString(MoveTemp(JoinedItems));
}