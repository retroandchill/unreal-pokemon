#include "RetroLib/Utils/StringUtilities.h"
#include "TestAdapter.h"
#include "Misc/AutomationTest.h"

TEST_CASE_NAMED(FStringUtilsTest, "Unit Tests::RetroLib::StringUtilities", "[RetroLib][Strings]") {

    FString Word1 = TEXT("Jolene");
    FText Word2 = FText::FromStringView(TEXT("Ezra"));
    CHECK_FALSE(UStringUtilities::StartsWithVowel(TEXT("")));
    CHECK_FALSE(UStringUtilities::StartsWithVowelString(Word1));
    CHECK(UStringUtilities::StartsWithVowelText(Word2));
}