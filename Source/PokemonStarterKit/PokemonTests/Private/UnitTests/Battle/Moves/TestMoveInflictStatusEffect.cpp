#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "TestAdapter.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

class IPokemon;

BEGIN_DEFINE_SPEC(FTestMoveInflictStatusEffect, "Unit Tests.Battle.Moves.TestMoveInflictStatusEffect",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
CORO_FUNCTIONS()
END_DEFINE_SPEC(FTestMoveInflictStatusEffect);

void FTestMoveInflictStatusEffect::Define() {
    CoroIt("MainEffect", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("CHANDELURE"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("MODEST"),
                                     .Moves = {{.Move = TEXT("WILLOWISP")}}});
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
        AddExpectedMessage(TEXT("Applying effects of Will-o-Wisp to Snorlax!"), ELogVerbosity::Display);
        AddExpectedMessage(TEXT("Snorlax didn't take any damage, skipping the additional effect chance!"),
                           ELogVerbosity::Display);
        co_await Action.Execute();

        CO_REQUIRE(Battler2->GetStatusEffect().IsSet());
        UE_CHECK_EQUAL(TEXT("BURN"), Battler2->GetStatusEffect()->StatusEffectID.ToString());
    });

    CoroIt("AlreadyHasStatus", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("CHANDELURE"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("MODEST"),
                                     .Moves = {{.Move = TEXT("WILLOWISP")}}});
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
        Battler2->InflictStatusEffect("BURN", FActiveGameplayEffectHandle());

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Will-o-Wisp failed against all targets!"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("AdditionalEffect", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("TOGEKISS"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("MODEST"),
                                     .Ability = FName("SERENEGRACE"),
                                     .Moves = {{.Move = TEXT("FIREBLAST")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                     .Nature = FName("CAREFUL")});

        FTemporarySeed Seed(156157);

        // Forcibly spin the RNG a few times to get favorable rolls
        for (int32 i = 0; i < 5; i++) {
            [[maybe_unused]] int32 Roll = FMath::Rand() % 100;
        }

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Fire Blast's additional effect chance against Snorlax calculated to be 20"),
                           ELogVerbosity::Display);
        AddExpectedMessage(TEXT("Applying additional effect of Fire Blast to Snorlax"), ELogVerbosity::Display);
        co_await Action.Execute();

        CO_REQUIRE(Battler2->GetStatusEffect().IsSet());
        UE_CHECK_EQUAL(TEXT("BURN"), Battler2->GetStatusEffect()->StatusEffectID.ToString());
    });

    CoroIt("AdditionalEffectBlocked", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("TOGEKISS"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("MODEST"),
                                     .Ability = FName("SERENEGRACE"),
                                     .Moves = {{.Move = TEXT("FIREBLAST")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("RIBOMBEE"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                     .Nature = FName("CAREFUL"),
                                                                                     .Ability = FName("SHIELDDUST")});

        FTemporarySeed Seed(156157);

        // Forcibly spin the RNG a few times to get favorable rolls
        for (int32 i = 0; i < 5; i++) {
            [[maybe_unused]] int32 Roll = FMath::Rand() % 100;
        }

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Fire Blast's additional effect chance against Ribombee calculated to be 0"),
                           ELogVerbosity::Display);
        AddExpectedMessage(TEXT("Fire Blast's additional effect chance is 0, skipping!"), ELogVerbosity::Display);
        co_await Action.Execute();

        UE_CHECK_FALSE(Battler2->GetStatusEffect().IsSet());
    });
}