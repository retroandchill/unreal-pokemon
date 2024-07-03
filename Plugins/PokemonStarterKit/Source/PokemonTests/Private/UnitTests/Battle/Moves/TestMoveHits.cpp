#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Misc/AutomationTest.h"
#include "Battle/Battlers/Battler.h"
#include "Lookup/InjectionUtilities.h"
#include "Mocks/MockBattler.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"
#include "Pokemon/Pokemon.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Certain, "Unit Tests.Battle.TestMoveHits.CertainHit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Certain::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("LUCARIO"),
            .Level = 100,
            .IVs = {{"SPECIAL_ATTACK", 31}},
            .EVs = {{"SPECIAL_ATTACK", 252}},
            .Nature = FName("MODEST"),
            .Moves = { TEXT("AURASPHERE") }
        });
    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("SNORLAX"),
            .Level = 100,
            .IVs = {{"SPECIAL_DEFENSE", 31}},
            .EVs = {{"SPECIAL_DEFENSE", 152}},
            .Nature = FName("CAREFUL")
        });

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, Pokemon1, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, Pokemon2, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Aura Sphere always hits, bypassing accuracy check against Snorlax"), ELogVerbosity::Display);
    Action.Execute();
    
    UE_CHECK_TRUE(Action.IsComplete());
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Regular, "Unit Tests.Battle.TestMoveHits.MoveMissed",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Regular::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("LUGIA"),
            .Level = 100,
            .IVs = {{"SPECIAL_ATTACK", 31}},
            .EVs = {{"SPECIAL_ATTACK", 252}},
            .Nature = FName("MODEST"),
            .Moves = { TEXT("HYDROPUMP") }
        });
    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("SNORLAX"),
            .Level = 100,
            .IVs = {{"SPECIAL_DEFENSE", 31}},
            .EVs = {{"SPECIAL_DEFENSE", 152}},
            .Nature = FName("CAREFUL")
        });

    FTemporarySeed Seed(6413);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, Pokemon1, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, Pokemon2, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Hydro Pump missed Snorlax!"), ELogVerbosity::Display);
    Action.Execute();
    
    UE_CHECK_TRUE(Action.IsComplete());
    UE_CHECK_EQUAL(1.f, Battler2->GetHPPercent());
    
    return true;
}