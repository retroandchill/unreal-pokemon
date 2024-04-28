// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Moves/DefaultMove.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveTest, "Unit Tests.Core.Moves.DefaultMoveTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveTest::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMove>()->Initialize(TEXT("SWORDSDANCE"));
    CHECK_EQUAL(20, Move->GetCurrentPP());
    CHECK_EQUAL(20, Move->GetTotalPP());
    return true;
}
#endif
