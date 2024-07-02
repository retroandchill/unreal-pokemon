#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"
#include "Pokemon/Pokemon.h"

class IPokemon;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysWeakNoCrit, "Unit Tests.Battle.Moves.TestDamageCalculation.PhysicalWeakNoCrit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysWeakNoCrit::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("GLACEON"),
            .Level = 75,
            .IVs = {{"ATTACK", 31}},
            .EVs = {{"ATTACK", 104}},
            .Nature = FName("TIMID"),
            .Moves = { TEXT("ICEFANG") }
        });
    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("GARCHOMP"),
            .Level = 65,
            .IVs = {{"DEFENSE", 31}},
            .EVs = {{"DEFENSE", 92}},
            .Nature = FName("JOLLY")
        });

    FTemporarySeed Seed(5661526);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, Pokemon1, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, Pokemon2, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Garchomp calculated to take 170 damage"), ELogVerbosity::Display);
    Action.Execute();
    UE_CHECK_TRUE(Action.IsComplete());
    
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDamageCalculation_PhysWeakWithCrit, "Unit Tests.Battle.Moves.TestDamageCalculation.PhysicalWeakWithCrit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDamageCalculation_PhysWeakWithCrit::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("GLACEON"),
            .Level = 75,
            .IVs = {{"ATTACK", 31}},
            .EVs = {{"ATTACK", 104}},
            .Nature = FName("TIMID"),
            .Item  = FName("MUSCLEBAND"),
            .Moves = { TEXT("ICEFANG") }
        });
    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{
            .Species = TEXT("GARCHOMP"),
            .Level = 65,
            .IVs = {{"DEFENSE", 31}},
            .EVs = {{"DEFENSE", 92}},
            .Nature = FName("JOLLY")
        });

    FTemporarySeed Seed(151566);
    // Roll 29 random numbers so that the next one %25 is 0
    for (int32 i = 0; i < 29; i++) {
        int32 Unused = FMath::Rand() % 24;
    }

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, Pokemon1, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, Pokemon2, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Garchomp calculated to take 279 damage"), ELogVerbosity::Display);
    Action.Execute();
    UE_CHECK_TRUE(Action.IsComplete());
    
    return true;
}