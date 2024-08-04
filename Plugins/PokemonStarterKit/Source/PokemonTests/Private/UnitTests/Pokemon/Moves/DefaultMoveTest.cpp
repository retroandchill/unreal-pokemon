// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Pokemon/PokemonDTO.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveTest, "Unit Tests.Core.Moves.DefaultMoveTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveTest::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMove>()->Initialize({.Move = TEXT("FLAMETHROWER")});
    UE_CHECK_EQUAL(90, Move->GetBasePower());
    UE_CHECK_EQUAL(100, Move->GetAccuracy());
    UE_CHECK_EQUAL(EMoveDamageCategory::Special, Move->GetDamageCategory());
    UE_CHECK_EQUAL(TEXT("BurnTarget"), Move->GetFunctionCode().ToString());
    UE_CHECK_EQUAL(TEXT("NearOther"), Move->GetTargetType().ID.ToString());
    UE_CHECK_EQUAL(15, Move->GetCurrentPP());
    UE_CHECK_EQUAL(15, Move->GetTotalPP());

    Move->DecrementPP(2);
    UE_CHECK_EQUAL(13, Move->GetCurrentPP());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestDefaultMoves, "Unit Tests.Core.Moves.DefaultMoveBlockTest.Default",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestDefaultMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize(nullptr, {.Species = "RIOLU", .Level = 40});
    auto Moves = Move->GetMoves();
    UE_ASSERT_EQUAL(4, Moves.Num());
    UE_CHECK_EQUAL("SCREECH", Moves[0]->GetMoveData().ID.ToString());
    UE_CHECK_EQUAL("QUICKGUARD", Moves[1]->GetMoveData().ID.ToString());
    UE_CHECK_EQUAL("FORCEPALM", Moves[2]->GetMoveData().ID.ToString());
    UE_CHECK_EQUAL("SWORDSDANCE", Moves[3]->GetMoveData().ID.ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestManualMoves, "Unit Tests.Core.Moves.DefaultMoveBlockTest.Manual",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestManualMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize(
        nullptr, {.Species = "RIOLU", .Level = 40, .Moves = {{.Move = "WORKUP"}, {.Move = "AURASPHERE"}, {.Move = "FEINT"}}});
    auto Moves = Move->GetMoves();
    UE_ASSERT_EQUAL(3, Moves.Num());
    UE_CHECK_EQUAL("WORKUP", Moves[0]->GetMoveData().ID.ToString());
    UE_CHECK_EQUAL("AURASPHERE", Moves[1]->GetMoveData().ID.ToString());
    UE_CHECK_EQUAL("FEINT", Moves[2]->GetMoveData().ID.ToString());
    return true;
}
