// "Unreal Pokémon" created by Retro & Chill.

#include "Strings/StringUtilities.h"
#include "RetroLib/Ranges/Compatibility/Array.h"
#include "RetroLib/Ranges/Views/JoinWith.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

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

    constexpr auto ExtractString = [](const FText &Text) -> const FString & { return Text.ToString(); };

    if (Items.Num() <= 2) {
        // clang-format off
        return FText::FromString(Items |
                                 Retro::Ranges::Views::Transform(ExtractString) |
                                 Retro::Ranges::Views::JoinWith(Conjunction.ToString()) |
                                 Retro::Ranges::To<FString>());
        // clang-format on
    }

    // clang-format off
    auto JoinedItems = Retro::TSpan<const FText>(Items.GetData(), Items.Num() - 1) |
                       Retro::Ranges::Views::Transform(ExtractString) |
                       Retro::Ranges::Views::JoinWith(Conjunction.ToString()) |
                       Retro::Ranges::To<FString>();
    // clang-format on
    if (bOxfordComma) {
        JoinedItems.Append(TEXT(", "));
    }

    JoinedItems.Appendf(TEXT(" %s %s"), *Conjunction.ToString(), *Items.Last().ToString());
    return FText::FromString(std::move(JoinedItems));
}

TSharedPtr<FString> UStringUtilities::NameToStringPtr(FName Name) {
    return MakeShared<FString>(Name.ToString());
}

FString UStringUtilities::FormatDate(const FDateTime &DateTime, const FString &Format) {
    return DateTime.ToFormattedString(*Format);
}

FText UStringUtilities::JoinText(const TArray<FText> &Lines) {
    // clang-format off
    return FText::FromString(Lines |
                             Retro::Ranges::Views::Transform<&FText::ToString>() |
                             Retro::Ranges::Views::JoinWith(TEXT("\n")) |
                             Retro::Ranges::To<FString>());
    // clang-format on
}