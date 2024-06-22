#include "Asserts.h"
#include "Battle/Moves/MoveLookup.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveEffectLookup, "Unit Tests.Battle.TestMoveEffectLookup",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveEffectLookup::RunTest(const FString &Parameters) {
    auto MoveClass = Battle::Moves::LookupMoveEffectClass("AlwaysCriticalHit");
    UE_CHECK_EQUAL(TEXT("Move_AlwaysCriticalHit_C"), MoveClass->GetName());

    MoveClass = Battle::Moves::LookupMoveEffectClass("NotARealMoveEffect");
    UE_CHECK_EQUAL(TEXT("BaseBattleMove"), MoveClass->GetName());
    return true;
}