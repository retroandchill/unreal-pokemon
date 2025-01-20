#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"
#include "TestAdapter.h"

BEGIN_DEFINE_SPEC(FTestDamageCalculation, "Unit Tests.Battle.Moves.TestDamageCalculation",
                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);
CORO_FUNCTIONS()

END_DEFINE_SPEC(FTestDamageCalculation);

void FTestDamageCalculation::Define() {
    CoroIt("PhysicalWeakNoCrit", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GLACEON"),
                                                                                     .Level = 75,
                                                                                     .IVs = {{"ATTACK", 31}},
                                                                                     .EVs = {{"ATTACK", 104}},
                                                                                     .Nature = FName("TIMID"),
                                                                                     .Moves = {{.Move = TEXT("ICEFANG")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GARCHOMP"),
                                                                                     .Level = 65,
                                                                                     .IVs = {{"DEFENSE", 31}},
                                                                                     .EVs = {{"DEFENSE", 92}},
                                                                                     .Nature = FName("JOLLY")});

        FTemporarySeed Seed(5661526);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Garchomp calculated to take 170 damage"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("PhysicalWeakWithCrit", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GLACEON"),
                                                                                     .Level = 75,
                                                                                     .IVs = {{"ATTACK", 31}},
                                                                                     .EVs = {{"ATTACK", 104}},
                                                                                     .Nature = FName("TIMID"),
                                                                                     .Item = FName("MUSCLEBAND"),
                                                                                     .Moves = {{.Move = TEXT("ICEFANG")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("GARCHOMP"),
                                                                                     .Level = 65,
                                                                                     .IVs = {{"DEFENSE", 31}},
                                                                                     .EVs = {{"DEFENSE", 92}},
                                                                                     .Nature = FName("JOLLY")});

        FTemporarySeed Seed(151566);
        // Roll 29 random numbers so that the next one %25 is 0
        for (int32 i = 0; i < 29; i++) {
            int32 Unused = FMath::Rand() % 24;
        }

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetAttackStagesAttribute(), -2.f);
        auto Battler2 = Side2->GetBattlers()[0];
        Battler2->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetDefenseStagesAttribute(), 2.f);

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Critical hit against Garchomp!"), ELogVerbosity::Display);
        AddExpectedMessage(TEXT("Garchomp calculated to take 279 damage"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("SpecialResistWithCrit", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("MEW"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("TIMID"),
                                     .Moves = {{.Move = TEXT("FROSTBREATH")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("MILOTIC"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 140}},
                                                                                     .Nature = FName("CALM")});

        FTemporarySeed Seed(48612);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Critical hit against Milotic!"), ELogVerbosity::Display);
        AddExpectedMessage(TEXT("Milotic calculated to take 28 damage"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("SpecialWithBlockedCrit", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("MEW"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("TIMID"),
                                     .Item = FName("TWISTEDSPOON"),
                                     .Moves = {{.Move = TEXT("FROSTBREATH")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("DRAPION"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 0}},
                                                                                     .Nature = FName("HARDY"),
                                                                                     .Ability = FName("BATTLEARMOR")});

        FTemporarySeed Seed(5512562);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Drapion calculated to take 79 damage"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("NoEffect", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("LATIOS"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("TIMID"),
                                     .Moves = {{.Move = TEXT("DRACOMETEOR")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SYLVEON"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 0}},
                                                                                     .Nature = FName("HARDY")});

        FTemporarySeed Seed(151261);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Sylveon is unaffected by Draco Meteor!"), ELogVerbosity::Display);
        co_await Action.Execute();

        UE_CHECK_EQUAL(1.f, Battler2->GetHPPercent());
    });

    CoroIt("SpecifiesSpecificBoost", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("LATIOS"),
                                     .Level = 100,
                                     .IVs = {{"SPECIAL_ATTACK", 31}},
                                     .EVs = {{"SPECIAL_ATTACK", 252}},
                                     .Nature = FName("TIMID"),
                                     .Item = FName("SOULDEW"),
                                     .Moves = {{.Move = TEXT("DRACOMETEOR")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("DRUDDIGON"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                     .EVs = {{"SPECIAL_DEFENSE", 0}},
                                                                                     .Nature = FName("HARDY")});

        FTemporarySeed Seed(82513);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Druddigon calculated to take 631"), ELogVerbosity::Display);
        co_await Action.Execute();
    });

    CoroIt("HighCriticalHitRate", [this]() -> UE5Coro::TCoroutine<> {
        auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
        auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
            World.Get(), FPokemonDTO{.Species = TEXT("HONCHKROW"),
                                     .Level = 100,
                                     .IVs = {{"ATTACK", 31}},
                                     .EVs = {{"ATTACK", 252}},
                                     .Nature = FName("TIMID"),
                                     .Ability = FName("SUPERLUCK"),
                                     .Item = FName("SCOPELENS"),
                                     .Moves = {{.Move = TEXT("NIGHTSLASH")}}});
        auto Pokemon2 =
            UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("ALAKAZAM"),
                                                                                     .Level = 100,
                                                                                     .IVs = {{"DEFENSE", 31}},
                                                                                     .EVs = {{"DEFENSE", 0}},
                                                                                     .Nature = FName("HARDY")});

        FTemporarySeed Seed(155612);

        auto Battle = World->SpawnActor<ATestPokemonBattle>();
        auto Side1 = World->SpawnActor<ATestActiveSide>();
        co_await Side1->Initialize(Battle, {Pokemon1}, false);
        auto Side2 = World->SpawnActor<ATestActiveSide>();
        co_await Side2->Initialize(Battle, {Pokemon2}, false);
        Battle->Initialize({Side1, Side2});

        auto Battler1 = Side1->GetBattlers()[0];
        auto Battler2 = Side2->GetBattlers()[0];

        FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
        AddExpectedMessage(TEXT("Critical hit against Alakazam!"), ELogVerbosity::Display);
        co_await Action.Execute();
    });
}