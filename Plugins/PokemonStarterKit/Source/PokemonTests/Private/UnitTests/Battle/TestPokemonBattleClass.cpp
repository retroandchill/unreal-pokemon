#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattleAction.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionSorting,
                                 "Unit Tests.Battle.TestPokemonBattleClass.ActionSorting",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionSorting::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    CREATE_MOCK(IBattleSide, Side1, FMockBattleSide, MockSide1);
    CREATE_MOCK(IBattleSide, Side2, FMockBattleSide, MockSide2);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto &ActionQueue = const_cast<TQueue<TUniquePtr<IBattleAction>>&>(Battle->GetActionQueue());
    auto &Actions = const_cast<TArray<TUniquePtr<IBattleAction>>&>(Battle->GetActions());
    TArray<IBattleAction*> ActionPointers;
    auto MockAction1 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction1, GetPriority).WillByDefault(Return(2));
    ActionPointers.Add(MockAction1.Get());
    Actions.Emplace(MoveTemp(MockAction1));

    auto MockAction2 = MakeUnique<FMockBattleAction>();
    CREATE_MOCK(IBattler, Battler2, FMockBattler, MockBattler2);
    ON_CALL(*MockAction2, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction2, GetBattler).WillByDefault(ReturnRef(Battler2));
    ON_CALL(MockBattler2, GetSpeed).WillByDefault(Return(FMainBattleStat{20, 0}));
    ActionPointers.Add(MockAction2.Get());
    Actions.Emplace(MoveTemp(MockAction2));

    auto MockAction3 = MakeUnique<FMockBattleAction>();
    CREATE_MOCK(IBattler, Battler3, FMockBattler, MockBattler3);
    ON_CALL(*MockAction3, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction3, GetBattler).WillByDefault(ReturnRef(Battler3));
    ON_CALL(MockBattler3, GetSpeed).WillByDefault(Return(FMainBattleStat{60, 0}));
    ActionPointers.Add(MockAction3.Get());
    Actions.Emplace(MoveTemp(MockAction3));

    auto& Phase = UReflectionUtils::GetMutablePropertyValue<EBattlePhase>(Battle, "Phase");
    Phase = EBattlePhase::Selecting;
    FActorTickFunction TickFunction;
    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_CHECK_EQUAL(EBattlePhase::Actions, Phase);

    auto &Action1 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action1.Get() == ActionPointers[0]);
    ActionQueue.Pop();

    auto &Action2 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action2.Get() == ActionPointers[2]);
    ActionQueue.Pop();

    auto &Action3 = *ActionQueue.Peek();
    UE_CHECK_TRUE(Action3.Get() == ActionPointers[1]);
    ActionQueue.Pop();

    UE_CHECK_TRUE(ActionQueue.IsEmpty());
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestPokemonBattleClass_ActionExecution,
                                 "Unit Tests.Battle.TestPokemonBattleClass.ActionExecution",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestPokemonBattleClass_ActionExecution::RunTest(const FString &Parameters) {
    /*
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    CREATE_MOCK(IBattleSide, Side1, FMockBattleSide, MockSide1);
    CREATE_MOCK(IBattleSide, Side2, FMockBattleSide, MockSide2);

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    auto& Phase = UReflectionUtils::GetMutablePropertyValue<EBattlePhase>(Battle, "Phase");
    Phase = EBattlePhase::Actions;
    FActorTickFunction TickFunction;
    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_CHECK_EQUAL(EBattlePhase::Judging, Phase);

    Phase = EBattlePhase::Actions;
    auto &ActionQueue = const_cast<TQueue<TUniquePtr<IBattleAction>>&>(Battle->GetActionQueue());
    auto MockAction1 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction1, CanExecute).WillByDefault(Return(false));
    ON_CALL(*MockAction1, IsExecuting).WillByDefault(Return(false));
    ActionQueue.Enqueue(MoveTemp(MockAction1));

    auto &bActionMessagesDisplayed = UReflectionUtils::GetMutablePropertyValue<bool>(Battle, "bActionMessagesDisplayed");
    auto MockAction2 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction2, CanExecute).WillByDefault(Return(true));
    EXPECT_CALL(*MockAction2, IsExecuting).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(*MockAction2, GetActionMessage).WillByDefault(Return(FText::GetEmpty()));
    ON_CALL(*MockAction2, Execute).WillByDefault([&bActionMessagesDisplayed]() { bActionMessagesDisplayed = true; });

    CREATE_MOCK(IBattler, Target, FMockBattler, MockTarget);
    FActionResult Result;
    Result.TargetResults.Add({.Target = Target});
    auto ResultFuture = AsyncThread([&Result] { return Result; });
    ON_CALL(*MockAction2, GetActionResult).WillByDefault(ReturnRef(ResultFuture));
    auto Action2 = MockAction2.Get();
    ActionQueue.Enqueue(MoveTemp(MockAction2));

    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_ASSERT_TRUE(ActionQueue.Peek()->Get() == Action2);
    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_CHECK_TRUE(bActionMessagesDisplayed);

    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    auto bActionResultDisplaying = UReflectionUtils::GetPropertyValue<bool>(Battle, "bActionResultDisplaying");
    UE_CHECK_TRUE(bActionResultDisplaying);

    ActionQueue.Empty();
    */
    return true;
}