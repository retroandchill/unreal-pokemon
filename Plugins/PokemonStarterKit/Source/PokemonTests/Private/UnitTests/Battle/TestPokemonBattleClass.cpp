#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "TestPokemonBattle.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace fakeit;
using namespace accessor;

MEMBER_ACCESSOR(AccessPhase, APokemonBattle, Phase, EBattlePhase)
MEMBER_ACCESSOR(AccessActions, APokemonBattle, SelectedActions, TArray<TUniquePtr<IBattleAction>>)
MEMBER_ACCESSOR(AccessActionQueue, APokemonBattle, ActionQueue, TQueue<TUniquePtr<IBattleAction>>)
MEMBER_ACCESSOR(AccessActionMessagesDisplayed, APokemonBattle, bActionMessagesDisplayed, bool)
MEMBER_ACCESSOR(AccessActionResultDisplaying, APokemonBattle, bActionResultDisplaying, bool)
FUNCTION_ACCESSOR(AccessBattleTick, AActor, Tick, void, float)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionSorting,
                                 "Private.UnitTests.Battle.TestPokemonBattleClass.ActionSorting",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionSorting::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto [Side1, MockSide1] = UnrealMock::CreateMock<IBattleSide>();
    auto [Side2, MockSide2] = UnrealMock::CreateMock<IBattleSide>();

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto &ActionQueue = accessMember<AccessActionQueue>(*Battle).get();
    auto &Actions = accessMember<AccessActions>(*Battle).get();
    auto MockAction1 = Mock<IBattleAction>();
    Fake(Dtor(MockAction1));
    When(Method(MockAction1, GetPriority)).AlwaysReturn(2);
    Actions.Emplace(&MockAction1.get());

    auto MockAction2 = Mock<IBattleAction>();
    Fake(Dtor(MockAction2));
    auto [Battler2, MockBattler2] = UnrealMock::CreateMock<IBattler>();
    When(Method(MockAction2, GetPriority)).AlwaysReturn(0);
    When(Method(MockAction2, GetBattler)).AlwaysReturn(Battler2);
    When(Method(MockBattler2, GetSpeed)).AlwaysReturn(20);
    Actions.Emplace(&MockAction2.get());

    auto MockAction3 = Mock<IBattleAction>();
    Fake(Dtor(MockAction3));
    auto [Battler3, MockBattler3] = UnrealMock::CreateMock<IBattler>();
    When(Method(MockAction3, GetPriority)).AlwaysReturn(0);
    When(Method(MockAction3, GetBattler)).AlwaysReturn(Battler3);
    When(Method(MockBattler3, GetSpeed)).AlwaysReturn(60);
    Actions.Emplace(&MockAction3.get());

    auto &Phase = accessMember<AccessPhase>(*Battle).get();
    Phase = EBattlePhase::Selecting;
    callFunction<AccessBattleTick>(*Battle, 1);
    CHECK_EQUAL(EBattlePhase::Actions, Phase);

    auto &Action1 = *ActionQueue.Peek();
    CHECK_TRUE(Action1.Get() == &MockAction1.get());
    ActionQueue.Pop();

    auto &Action2 = *ActionQueue.Peek();
    CHECK_TRUE(Action2.Get() == &MockAction3.get());
    ActionQueue.Pop();

    auto &Action3 = *ActionQueue.Peek();
    CHECK_TRUE(Action3.Get() == &MockAction2.get());
    ActionQueue.Pop();

    CHECK_TRUE(ActionQueue.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionExecution,
                                 "Private.UnitTests.Battle.TestPokemonBattleClass.ActionExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionExecution::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto [Side1, MockSide1] = UnrealMock::CreateMock<IBattleSide>();
    auto [Side2, MockSide2] = UnrealMock::CreateMock<IBattleSide>();

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto &Phase = accessMember<AccessPhase>(*Battle).get();
    Phase = EBattlePhase::Actions;
    callFunction<AccessBattleTick>(*Battle, 1);
    CHECK_EQUAL(EBattlePhase::Judging, Phase);

    Phase = EBattlePhase::Actions;
    auto &ActionQueue = accessMember<AccessActionQueue>(*Battle).get();
    auto MockAction1 = Mock<IBattleAction>();
    Fake(Dtor(MockAction1));
    When(Method(MockAction1, CanExecute)).AlwaysReturn(false);
    When(Method(MockAction1, IsExecuting)).AlwaysReturn(false);
    ActionQueue.Enqueue(TUniquePtr<IBattleAction>(&MockAction1.get()));

    auto &bActionMessagesDisplayed = accessMember<AccessActionMessagesDisplayed>(*Battle).get();
    auto MockAction2 = Mock<IBattleAction>();
    Fake(Dtor(MockAction2));
    When(Method(MockAction2, CanExecute)).AlwaysReturn(true);
    When(Method(MockAction2, IsExecuting)).Return(false).Return(true);
    When(Method(MockAction2, GetActionMessage)).AlwaysReturn(FText::GetEmpty());
    When(Method(MockAction2, Execute)).AlwaysDo([&bActionMessagesDisplayed] { bActionMessagesDisplayed = true; });

    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler>();
    Fake(Method(MockTarget, TakeBattleDamage));
    FActionResult Result;
    Result.TargetResults.Add({.Target = Target});
    auto ResultFuture = AsyncThread([&Result] { return Result; });
    When(Method(MockAction2, GetActionResult)).AlwaysReturn(ResultFuture);
    ActionQueue.Enqueue(TUniquePtr<IBattleAction>(&MockAction2.get()));

    callFunction<AccessBattleTick>(*Battle, 1);
    ASSERT_TRUE(ActionQueue.Peek()->Get() == &MockAction2.get());
    callFunction<AccessBattleTick>(*Battle, 1);
    CHECK_TRUE(bActionMessagesDisplayed);

    callFunction<AccessBattleTick>(*Battle, 1);
    auto &bActionResultDisplaying = accessMember<AccessActionResultDisplaying>(*Battle).get();
    CHECK_TRUE(bActionResultDisplaying);

    ActionQueue.Empty();
    return true;
}