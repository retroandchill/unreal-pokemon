#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "TestAdapter.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

BEGIN_DEFINE_SPEC(FTestStatChangingMoves, "Unit Tests.Battle.Moves.TestStatChangingMoves",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
CORO_FUNCTIONS()
END_DEFINE_SPEC(FTestStatChangingMoves);

void FTestStatChangingMoves::Define()
{
    CoroIt("RaiseUserStats.Success", [this]() -> UE5Coro::TCoroutine<> {
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
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {});
        co_await Action.Execute();

        UE_CHECK_EQUAL(2.f, Battler1->GetAbilityComponent()->GetStatStages()->GetSpecialAttackStages());
    });

    CoroIt("RaiseUserStats.Failed", [this]() -> UE5Coro::TCoroutine<> {
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
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        Battler1->GetAbilityComponent()->SetNumericAttributeBase(
            UStatStagesAttributeSet::GetSpecialAttackStagesAttribute(), 6.f);

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {});
        AddExpectedMessage(TEXT("Nasty Plot failed!"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("LowerUserStats", [this]() -> UE5Coro::TCoroutine<> {
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
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        co_await Action.Execute();

        UE_CHECK_EQUAL(-1.f, Battler1->GetAbilityComponent()->GetStatStages()->GetDefenseStages());
        UE_CHECK_EQUAL(-1.f, Battler1->GetAbilityComponent()->GetStatStages()->GetSpecialDefenseStages());
    });

    CoroIt("LowerTargetStats.Success", [this]() -> UE5Coro::TCoroutine<> {
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
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        co_await Action.Execute();

        UE_CHECK_EQUAL(-2.f, Battler2->GetAbilityComponent()->GetStatStages()->GetSpecialDefenseStages());
    });

    CoroIt("LowerTargetStats.Failed", [this]() -> UE5Coro::TCoroutine<> {
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
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];
        Battler2->GetAbilityComponent()->SetNumericAttributeBase(
            UStatStagesAttributeSet::GetSpecialDefenseStagesAttribute(), -6.f);

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Metal Sound failed against all targets!"), ELogVerbosity::Display);
        co_await Action.Execute();
    });
}