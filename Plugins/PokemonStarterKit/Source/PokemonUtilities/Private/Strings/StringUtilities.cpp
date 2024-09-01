// "Unreal Pokémon" created by Retro & Chill.

#include "Strings/StringUtilities.h"
#include "Ranges/Algorithm/ToString.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Map.h"
#include "Ranges/Views/Span.h"

bool UStringUtilities::StartsWithVowelText(const FText &Text) {
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

FText UStringUtilities::GenerateList(const TArray<FText> &Items, const FText &Conjunction, bool bOxfordComma) {
    if (Items.IsEmpty()) {
        return FText::GetEmpty();
    }

    auto ExtractString = [](const FText &Text) -> const FString & { return Text.ToString(); };

    if (Items.Num() <= 2) {
        // clang-format off
        return FText::FromString(Items |
            UE::Ranges::Map(ExtractString) |
            UE::Ranges::ToString(Conjunction.ToString()));
        // clang-format on
    }

    // clang-format off
    auto JoinedItems = UE::Ranges::TSpan<const FText>(Items.GetData(), Items.Num() - 1) |
                       UE::Ranges::Map(ExtractString) |
                           UE::Ranges::ToString(TEXT(", "));
        // clang-format on
    if (bOxfordComma) {
        JoinedItems.Append(TEXT(", "));
    }

    JoinedItems.Appendf(TEXT(" %s %s"), *Conjunction.ToString(), *Items.Last().ToString());
    return FText::FromString(MoveTemp(JoinedItems));
}