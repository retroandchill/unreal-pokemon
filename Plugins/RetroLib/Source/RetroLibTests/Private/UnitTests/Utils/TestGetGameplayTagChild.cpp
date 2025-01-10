
#include "Misc/AutomationTest.h"
#include "NativeGameplayTags.h"
#include "RetroLib/Utils/GameplayTagUtilities.h"
#include "TestAdapter.h"

namespace Retro::Testing {
    const UE_DEFINE_GAMEPLAY_TAG(LandEncounter, "EncounterTypes.Land");
}

TEST_CASE_NAMED(FTestGetGameplayTagChild, "Unit Tests::RetroLib::GameplayTags::GetGamplayTagChild",
                "[RetroLib][Utils]") {
    auto ResultTag = UGameplayTagUtilities::RequestGameplayTagChild(Retro::Testing::LandEncounter, "Fake");
    CHECK_FALSE(ResultTag.IsValid());
    ResultTag =
        UGameplayTagUtilities::RequestGameplayTagChild(FGameplayTag::RequestGameplayTag("EncounterTypes"), "Land");
    CHECK(ResultTag.IsValid());
}