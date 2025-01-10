
#include "TestAdapter.h"
#include "NativeGameplayTags.h"
#include "Misc/AutomationTest.h"
#include "RetroLib/Utils/GameplayTagUtilities.h"

namespace Retro::Testing {
    const UE_DEFINE_GAMEPLAY_TAG(LandEncounter, "EncounterTypes.Land");
}

TEST_CASE_NAMED(FTestGetGameplayTagChild, "Unit Tests::RetroLib::GameplayTags::GetGamplayTagChild", "[RetroLib][Utils]") {
        auto ResultTag = UGameplayTagUtilities::RequestGameplayTagChild(Retro::Testing::LandEncounter, "Fake");
        CHECK_FALSE(ResultTag.IsValid());
        ResultTag =
            UGameplayTagUtilities::RequestGameplayTagChild(FGameplayTag::RequestGameplayTag("EncounterTypes"), "Land");
        CHECK(ResultTag.IsValid());
}