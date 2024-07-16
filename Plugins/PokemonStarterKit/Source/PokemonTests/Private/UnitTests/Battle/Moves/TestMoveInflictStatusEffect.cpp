#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

class IPokemon;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveInflictStatusEffect_MainEffect,
                                 "Unit Tests.Battle.Moves.TestMoveInflictStatusEffect.MainEffect",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveInflictStatusEffect_MainEffect::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("CHANDELURE"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                                                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                                                                 .Nature = FName("MODEST"),
                                                                                 .Moves = {TEXT("WILLOWISP")}});
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

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Applying effects of Will-o-Wisp to Snorlax!"), ELogVerbosity::Display);
    AddExpectedMessage(TEXT("Snorlax didn't take any damage, skipping the additional effect chance!"),
                       ELogVerbosity::Display);
    Action.Execute();

    UE_CHECK_TRUE(Action.IsComplete());
    UE_ASSERT_TRUE(Battler2->GetStatusEffect().IsSet());
    UE_CHECK_EQUAL(TEXT("BURN"), Battler2->GetStatusEffect()->StatusEffectID.ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveInflictStatusEffect_AlreadyHas,
                                 "Unit Tests.Battle.Moves.TestMoveInflictStatusEffect.AlreadyHasStatus",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveInflictStatusEffect_AlreadyHas::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("CHANDELURE"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                                                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                                                                 .Nature = FName("MODEST"),
                                                                                 .Moves = {TEXT("WILLOWISP")}});
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
    Battler2->InflictStatusEffect("BURN", FActiveGameplayEffectHandle());

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Will-o-Wisp failed against all targets!"), ELogVerbosity::Display);
    Action.Execute();

    UE_CHECK_TRUE(Action.IsComplete());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveInflictStatusEffect_AdditionalEffect,
                                 "Unit Tests.Battle.Moves.TestMoveInflictStatusEffect.AdditionalEffect",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveInflictStatusEffect_AdditionalEffect::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("TOGEKISS"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                                                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                                                                 .Nature = FName("MODEST"),
                                                                                 .Ability = FName("SERENEGRACE"),
                                                                                 .Moves = {TEXT("FIREBLAST")}});
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
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Fire Blast's additional effect chance against Snorlax calculated to be 20"),
                       ELogVerbosity::Display);
    AddExpectedMessage(TEXT("Applying additional effect of Fire Blast to Snorlax"), ELogVerbosity::Display);
    Action.Execute();

    UE_CHECK_TRUE(Action.IsComplete());
    UE_ASSERT_TRUE(Battler2->GetStatusEffect().IsSet());
    UE_CHECK_EQUAL(TEXT("BURN"), Battler2->GetStatusEffect()->StatusEffectID.ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveInflictStatusEffect_AdditionalEffectBlocked,
                                 "Unit Tests.Battle.Moves.TestMoveInflictStatusEffect.AdditionalEffectBlocked",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveInflictStatusEffect_AdditionalEffectBlocked::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("TOGEKISS"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                                                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                                                                 .Nature = FName("MODEST"),
                                                                                 .Ability = FName("SERENEGRACE"),
                                                                                 .Moves = {TEXT("FIREBLAST")}});
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
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {Battler2});
    AddExpectedMessage(TEXT("Fire Blast's additional effect chance against Ribombee calculated to be 0"),
                       ELogVerbosity::Display);
    AddExpectedMessage(TEXT("Fire Blast's additional effect chance is 0, skipping!"), ELogVerbosity::Display);
    Action.Execute();

    UE_CHECK_TRUE(Action.IsComplete());
    UE_CHECK_FALSE(Battler2->GetStatusEffect().IsSet());

    return true;
}