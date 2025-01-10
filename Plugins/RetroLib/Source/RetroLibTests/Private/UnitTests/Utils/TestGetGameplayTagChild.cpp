
#include "Misc/AutomationTest.h"
#include "NativeGameplayTags.h"
#include "RetroLib/Utils/GameplayTagUtilities.h"
#include "TestAdapter.h"

TEST_CASE_NAMED(FTestGetGameplayTagChild, "Unit Tests::RetroLib::GameplayTags::GetGamplayTagChild",
                "[RetroLib][Utils]") {
    auto Tag = FGameplayTag::RequestGameplayTag("Tests.GenericTag.One");
    auto ResultTag = UGameplayTagUtilities::RequestGameplayTagChild(Tag, "Fake");
    CHECK_FALSE(ResultTag.IsValid());
    ResultTag =
        UGameplayTagUtilities::RequestGameplayTagChild(FGameplayTag::RequestGameplayTag("Tests.GenericTag"), "One");
    CHECK(ResultTag.IsValid());
}