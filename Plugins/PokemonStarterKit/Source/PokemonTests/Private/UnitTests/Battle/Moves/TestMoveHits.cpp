#include "Asserts.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocks/MockBattler.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Species/SpeciesData.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Certain, "Unit Tests.Battle.TestMoveHits.CertainHit",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Certain::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("AURASPHERE")}}});
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
    Battle->ClearOnBattleEnd();

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    AddExpectedMessage(TEXT("Aura Sphere always hits, bypassing accuracy check against Snorlax"),
                       ELogVerbosity::Display);
    Action.Execute();

    

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_Regular, "Unit Tests.Battle.TestMoveHits.MoveMissed",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_Regular::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUGIA"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("HYDROPUMP")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(6413);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});
    Battle->ClearOnBattleEnd();

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    AddExpectedMessage(TEXT("Hydro Pump missed Snorlax!"), ELogVerbosity::Display);
    Action.Execute();

    
    UE_CHECK_EQUAL(1.f, Battler2->GetHPPercent());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_AccuracyEvasionRegular,
                                 "Unit Tests.Battle.TestMoveHits.AccuracyEvasion.Regular",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_AccuracyEvasionRegular::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUGIA"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("HYDROPUMP")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(6413);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});
    Battle->ClearOnBattleEnd();

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];
    Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetAccuracyStagesAttribute(),
                                                             1.f);
    Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetEvasionStagesAttribute(),
                                                             -4.f);

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    AddExpectedMessage(TEXT("Accuracy threshold for Snorlax is 107"), ELogVerbosity::Display);
    Action.Execute();

    

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestMoveHits_AccuracyEvasionOutsideBounds,
                                 "Unit Tests.Battle.TestMoveHits.AccuracyEvasion.OutsideBounds",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestMoveHits_AccuracyEvasionOutsideBounds::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = TEXT("LUGIA"),
                                 .Level = 100,
                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                 .Nature = FName("MODEST"),
                                 .Moves = {{.Move = TEXT("HYDROPUMP")}}});
    auto Pokemon2 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("SNORLAX"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_DEFENSE", 31}},
                                                                                 .EVs = {{"SPECIAL_DEFENSE", 152}},
                                                                                 .Nature = FName("CAREFUL")});

    FTemporarySeed Seed(6413);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    auto Side1 = World->SpawnActor<ATestActiveSide>();
    Side1->Initialize(Battle, {Pokemon1}, false);
    auto Side2 = World->SpawnActor<ATestActiveSide>();
    Side2->Initialize(Battle, {Pokemon2}, false);
    Battle->Initialize({Side1, Side2});
    Battle->ClearOnBattleEnd();

    auto Battler1 = Side1->GetBattlers()[0];
    auto Battler2 = Side2->GetBattlers()[0];
    Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetAccuracyStagesAttribute(),
                                                             -6.f);
    Battler1->GetAbilityComponent()->SetNumericAttributeBase(UStatStagesAttributeSet::GetEvasionStagesAttribute(),
                                                             -6.f);

    FBattleActionUseMove Action(Battler1, Battler1->GetMoves()[0], {FTargetWithIndex(Battler2)});
    AddExpectedMessage(TEXT("Accuracy threshold for Snorlax is 27"), ELogVerbosity::Display);
    AddExpectedMessage(TEXT("Hydro Pump missed Snorlax!"), ELogVerbosity::Display);
    Action.Execute();

    
    UE_CHECK_EQUAL(1.f, Battler2->GetHPPercent());

    return true;
}