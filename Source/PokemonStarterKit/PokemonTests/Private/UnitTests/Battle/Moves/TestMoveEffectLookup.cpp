#include "Asserts.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Moves/MoveLookup.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveEffectLookup, "Unit Tests.Battle.Moves.TestMoveEffectLookup",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveEffectLookup::RunTest(const FString &Parameters)
{
    auto MoveClass = Pokemon::Battle::Moves::LookupMoveEffectClass("AlwaysCriticalHit");
    UE_CHECK_EQUAL(TEXT("Move_AlwaysCriticalHit_C"), MoveClass->GetName());

    MoveClass = Pokemon::Battle::Moves::LookupMoveEffectClass("NotARealMoveEffect");
    UE_CHECK_EQUAL(TEXT("BattleMoveFunctionCode"), MoveClass->GetName());

    MoveClass = Pokemon::Battle::Moves::LookupMoveEffectClass(NAME_None);
    UE_CHECK_EQUAL(TEXT("BattleMoveFunctionCode"), MoveClass->GetName());
    return true;
}