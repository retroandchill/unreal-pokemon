#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattleAction.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

using namespace testing;
using namespace accessor;

MEMBER_ACCESSOR(AccessPhase, APokemonBattle, Phase, EBattlePhase)
MEMBER_ACCESSOR(AccessActions, APokemonBattle, SelectedActions, TArray<TUniquePtr<IBattleAction>>)
MEMBER_ACCESSOR(AccessActionQueue, APokemonBattle, ActionQueue, TQueue<TUniquePtr<IBattleAction>>)
MEMBER_ACCESSOR(AccessActionMessagesDisplayed, APokemonBattle, bActionMessagesDisplayed, bool)
MEMBER_ACCESSOR(AccessActionResultDisplaying, APokemonBattle, bActionResultDisplaying, bool)
FUNCTION_ACCESSOR(AccessBattleTick, AActor, Tick, void, float)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionSorting,
                                 "Unit Tests.Battle.TestPokemonBattleClass.ActionSorting",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionSorting::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto [Side1, MockSide1] = UnrealMock::CreateMock<IBattleSide, FMockBattleSide>();
    auto [Side2, MockSide2] = UnrealMock::CreateMock<IBattleSide, FMockBattleSide>();

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto &ActionQueue = accessMember<AccessActionQueue>(*Battle).get();
    auto &Actions = accessMember<AccessActions>(*Battle).get();
    TArray<IBattleAction*> ActionPointers;
    auto MockAction1 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction1, GetPriority).WillByDefault(Return(2));
    ActionPointers.Add(MockAction1.Get());
    Actions.Emplace(MoveTemp(MockAction1));

    auto MockAction2 = MakeUnique<FMockBattleAction>();
    auto [Battler2, MockBattler2] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    ON_CALL(*MockAction2, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction2, GetBattler).WillByDefault(Return(Battler2));
    ON_CALL(MockBattler2, GetSpeed).WillByDefault(Return(20));
    ActionPointers.Add(MockAction2.Get());
    Actions.Emplace(MoveTemp(MockAction2));

    auto MockAction3 = MakeUnique<FMockBattleAction>();
    auto [Battler3, MockBattler3] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    ON_CALL(*MockAction3, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction3, GetBattler).WillByDefault(Return(Battler3));
    ON_CALL(MockBattler3, GetSpeed).WillByDefault(Return(60));
    ActionPointers.Add(MockAction3.Get());
    Actions.Emplace(MoveTemp(MockAction3));

    auto &Phase = accessMember<AccessPhase>(*Battle).get();
    Phase = EBattlePhase::Selecting;
    callFunction<AccessBattleTick>(*Battle, 1);
    UE_CHECK_EQUAL(EBattlePhase::Actions, Phase);

    auto &Action1 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action1.Get() == ActionPointers[0]);
    ActionQueue.Pop();

    auto &Action2 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action2.Get() == ActionPointers[1]);
    ActionQueue.Pop();

    auto &Action3 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action3.Get() == ActionPointers[2]);
    ActionQueue.Pop();

    UE_CHECK_TRUE(ActionQueue.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionExecution,
                                 "Unit Tests.Battle.TestPokemonBattleClass.ActionExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionExecution::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto [Side1, MockSide1] = UnrealMock::CreateMock<IBattleSide, FMockBattleSide>();
    auto [Side2, MockSide2] = UnrealMock::CreateMock<IBattleSide, FMockBattleSide>();

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto &Phase = accessMember<AccessPhase>(*Battle).get();
    Phase = EBattlePhase::Actions;
    callFunction<AccessBattleTick>(*Battle, 1);
    UE_CHECK_EQUAL(EBattlePhase::Judging, Phase);

    Phase = EBattlePhase::Actions;
    auto &ActionQueue = accessMember<AccessActionQueue>(*Battle).get();
    auto MockAction1 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction1, CanExecute).WillByDefault(Return(false));
    ON_CALL(*MockAction1, IsExecuting).WillByDefault(Return(false));
    ActionQueue.Enqueue(MoveTemp(MockAction1));

    auto &bActionMessagesDisplayed = accessMember<AccessActionMessagesDisplayed>(*Battle).get();
    auto MockAction2 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction2, CanExecute).WillByDefault(Return(true));
    EXPECT_CALL(*MockAction2, IsExecuting).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(*MockAction2, GetActionMessage).WillByDefault(Return(FText::GetEmpty()));
    ON_CALL(*MockAction2, Execute).WillByDefault([&bActionMessagesDisplayed] { bActionMessagesDisplayed = true; });

    auto [Target, MockTarget] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    FActionResult Result;
    Result.TargetResults.Add({.Target = Target});
    auto ResultFuture = AsyncThread([&Result] { return Result; });
    ON_CALL(*MockAction2, GetActionResult).WillByDefault(ReturnRef(ResultFuture));
    auto Action2 = MockAction2.Get();
    ActionQueue.Enqueue(MoveTemp(MockAction2));

    callFunction<AccessBattleTick>(*Battle, 1);
    UE_ASSERT_TRUE(ActionQueue.Peek()->Get() == Action2);
    callFunction<AccessBattleTick>(*Battle, 1);
    UE_CHECK_TRUE(bActionMessagesDisplayed);

    callFunction<AccessBattleTick>(*Battle, 1);
    auto &bActionResultDisplaying = accessMember<AccessActionResultDisplaying>(*Battle).get();
    UE_CHECK_TRUE(bActionResultDisplaying);

    ActionQueue.Empty();
    return true;
}