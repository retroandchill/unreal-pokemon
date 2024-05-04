// "Unreal Pokémon" created by Retro & Chill.
#include "Moves/DefaultMoveBlock.h"
#include "Moves/MoveData.h"
#include "Pokemon/PokemonDTO.h"
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Moves/DefaultMove.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveTest, "Unit Tests.Core.Moves.DefaultMoveTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveTest::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMove>()->Initialize(TEXT("SWORDSDANCE"));
    CHECK_EQUAL(20, Move->GetCurrentPP());
    CHECK_EQUAL(20, Move->GetTotalPP());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestDefaultMoves, "Unit Tests.Core.Moves.DefaultMoveBlockTest.Default",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestDefaultMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize({.Species = "RIOLU", .Level = 40});
    auto &Moves = Move->GetMoves();
    ASSERT_EQUAL(4, Moves.Num());
    CHECK_EQUAL("SCREECH", Moves[0]->GetMoveData().ID.ToString());
    CHECK_EQUAL("QUICKGUARD", Moves[1]->GetMoveData().ID.ToString());
    CHECK_EQUAL("FORCEPALM", Moves[2]->GetMoveData().ID.ToString());
    CHECK_EQUAL("SWORDSDANCE", Moves[3]->GetMoveData().ID.ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestManualMoves, "Unit Tests.Core.Moves.DefaultMoveBlockTest.Manual",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestManualMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize(
        {.Species = "RIOLU", .Level = 40, .Moves = {"WORKUP", "AURASPHERE", "FEINT"}});
    auto &Moves = Move->GetMoves();
    ASSERT_EQUAL(3, Moves.Num());
    CHECK_EQUAL("WORKUP", Moves[0]->GetMoveData().ID.ToString());
    CHECK_EQUAL("AURASPHERE", Moves[1]->GetMoveData().ID.ToString());
    CHECK_EQUAL("FEINT", Moves[2]->GetMoveData().ID.ToString());
    return true;
}

#endif
