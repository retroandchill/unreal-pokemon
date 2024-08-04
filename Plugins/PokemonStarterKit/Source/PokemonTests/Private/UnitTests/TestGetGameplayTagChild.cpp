#include "Asserts.h"
#include "EncounterData/BasicEncounterTypes.h"
#include "GameplayTags/GameplayTagUtilities.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestGetGameplayTagChild, "Private.UnitTests.TestGetGameplayTagChild",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestGetGameplayTagChild::RunTest(const FString &Parameters) {
    auto ResultTag = UGameplayTagUtilities::RequestGameplayTagChild(Pokemon::Encounters::Land, "Fake");
    UE_ASSERT_FALSE(ResultTag.IsValid());
    ResultTag =
        UGameplayTagUtilities::RequestGameplayTagChild(FGameplayTag::RequestGameplayTag("EncounterType"), "Land");
    UE_ASSERT_TRUE(ResultTag.IsValid());
    return true;
}