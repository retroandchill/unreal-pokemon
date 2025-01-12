#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChangingMoves_RaiseUserStats,
                                 "Unit Tests.Battle.Moves.TestStatChangingMoves.RaiseUserStats.Success",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChangingMoves_RaiseUserStats::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("NASTYPLOT")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {});
    Action.Execute();

    UE_CHECK_EQUAL(2.f, Battler1->GetAbilityComponent()->GetStatStages()->GetSpecialAttackStages());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChangingMoves_RaiseUserStats_Failed,
                                 "Unit Tests.Battle.Moves.TestStatChangingMoves.RaiseUserStats.Failed",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChangingMoves_RaiseUserStats_Failed::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("NASTYPLOT")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetSpecialAttackStagesAttribute(),
                                                             6.f);

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {});
    AddExpectedMessage(TEXT("Nasty Plot failed!"), ELogVerbosity::Display);
    Action.Execute();

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChangingMoves_LowerUserStats,
                                 "Unit Tests.Battle.Moves.TestStatChangingMoves.LowerUserStats",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChangingMoves_LowerUserStats::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("CLOSECOMBAT")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    Action.Execute();

    UE_CHECK_EQUAL(-1.f, Battler1->GetAbilityComponent()->GetStatStages()->GetDefenseStages());
    UE_CHECK_EQUAL(-1.f, Battler1->GetAbilityComponent()->GetStatStages()->GetSpecialDefenseStages());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChangingMoves_LowerTargetStats,
                                 "Unit Tests.Battle.Moves.TestStatChangingMoves.LowerTargetStats.Success",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChangingMoves_LowerTargetStats::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("METALSOUND")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    Action.Execute();

    UE_CHECK_EQUAL(-2.f, Battler2->GetAbilityComponent()->GetStatStages()->GetSpecialDefenseStages());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatChangingMoves_LowerTargetStats_Failed,
                                 "Unit Tests.Battle.Moves.TestStatChangingMoves.LowerTargetStats.Failed",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatChangingMoves_LowerTargetStats_Failed::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("METALSOUND")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(45126);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];
    Battler2->GetAbilityComponent()->SetNumericAttributeBase(
        UStatStagesAttributeSet::GetSpecialDefenseStagesAttribute(), -6.f);

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    AddExpectedMessage(TEXT("Metal Sound failed against all targets!"), ELogVerbosity::Display);
    Action.Execute();

    return true;
}