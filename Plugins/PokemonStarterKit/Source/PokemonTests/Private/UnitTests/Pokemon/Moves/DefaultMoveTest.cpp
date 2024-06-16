﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/DefaultMove.h"
#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Pokemon/PokemonDTO.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveTest, "Private.UnitTests.Core.Moves.DefaultMoveTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveTest::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMove>()->Initialize(TEXT("SWORDSDANCE"));
    CHECK_EQUAL(20, Move->GetCurrentPP());
    CHECK_EQUAL(20, Move->GetTotalPP());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestDefaultMoves,
                                 "Private.UnitTests.Core.Moves.DefaultMoveBlockTest.Default",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestDefaultMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize({.Species = "RIOLU", .Level = 40});
    auto Moves = Move->GetMoves();
    ASSERT_EQUAL(4, Moves.Num());
    CHECK_EQUAL("SCREECH", Moves[0]->GetMoveData().ID.ToString());
    CHECK_EQUAL("QUICKGUARD", Moves[1]->GetMoveData().ID.ToString());
    CHECK_EQUAL("FORCEPALM", Moves[2]->GetMoveData().ID.ToString());
    CHECK_EQUAL("SWORDSDANCE", Moves[3]->GetMoveData().ID.ToString());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultMoveBlockTestManualMoves,
                                 "Private.UnitTests.Core.Moves.DefaultMoveBlockTest.Manual",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool DefaultMoveBlockTestManualMoves::RunTest(const FString &Parameters) {
    auto Move = NewObject<UDefaultMoveBlock>()->Initialize(
        {.Species = "RIOLU", .Level = 40, .Moves = {"WORKUP", "AURASPHERE", "FEINT"}});
    auto Moves = Move->GetMoves();
    ASSERT_EQUAL(3, Moves.Num());
    CHECK_EQUAL("WORKUP", Moves[0]->GetMoveData().ID.ToString());
    CHECK_EQUAL("AURASPHERE", Moves[1]->GetMoveData().ID.ToString());
    CHECK_EQUAL("FEINT", Moves[2]->GetMoveData().ID.ToString());
    return true;
}
