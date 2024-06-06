#include "Strings/StringUtilities.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StringUtilitiesTest, "Private.StringUtilitiesTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool StringUtilitiesTest::RunTest(const FString &Parameters) {
    FString Word1 = TEXT("Jolene");
    FText Word2 = FText::FromStringView(TEXT("Ezra"));
    CHECK_FALSE(UStringUtilities::StartsWithVowel(TEXT("")));
    CHECK_FALSE(UStringUtilities::StartsWithVowelString(Word1));
    CHECK_TRUE(UStringUtilities::StartsWithVowelText(Word2));
    return true;
}