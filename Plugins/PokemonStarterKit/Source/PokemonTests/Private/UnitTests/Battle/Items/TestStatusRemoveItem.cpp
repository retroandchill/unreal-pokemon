#include "Asserts.h"
#include "Battle/Actions/BattleActionUseItem.h"
#include "Battle/Actions/BattleActionUseMove.h"
#include "Battle/Battlers/Battler.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/TemporarySeed.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestActiveSide.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestStatusRemoveItem, "Unit Tests.Battle.Items.TestStatusRemoveItem",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestStatusRemoveItem::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Pokemon1 =
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = TEXT("LUCARIO"),
                                                                                 .Level = 100,
                                                                                 .IVs = {{"SPECIAL_ATTACK", 31}},
                                                                                 .EVs = {{"SPECIAL_ATTACK", 252}},
                                                                                 .Nature = FName("MODEST"),
                                                                                 .StatusEffect = FName("BURN")});
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
    UE_ASSERT_TRUE(Battler1->GetStatusEffect().IsSet());
    UE_CHECK_EQUAL(TEXT("BURN"), Battler1->GetStatusEffect()->StatusEffectID.ToString());

    FBattleActionUseItem Action(Battler1, "BURNHEAL", FItemTarget(TWeakInterfacePtr<IBattler>(Battler1.GetObject())));
    Action.Execute();

    UE_ASSERT_FALSE(Battler1->GetStatusEffect().IsSet());

    return true;
}