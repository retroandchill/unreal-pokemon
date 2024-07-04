#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/BattleSideAbilitySystemComponent.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattleAction.h"
#include "Mocks/MockBattler.h"
#include "Mocks/MockBattleSide.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"
#include "Species/SpeciesData.h"

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
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Battler2, FMockBattler, MockBattler2);
    auto Battler2Actor = static_cast<AActor*>(Battler2.GetObject());
    auto Battler2Abilities = static_cast<UBattlerAbilityComponent*>(Battler2Actor->AddComponentByClass(UBattlerAbilityComponent::StaticClass(), false, FTransform(), false));
    Battler2Actor->DispatchBeginPlay();
    ON_CALL(MockBattler2, GetAbilityComponent).WillByDefault(Return(Battler2Abilities));
    ON_CALL(*MockAction2, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction2, GetBattler).WillByDefault(ReturnRef(Battler2));
    Battler2Abilities->GetCoreAttributes()->InitSpeed(20);
    ActionPointers.Add(MockAction2.Get());
    Actions.Emplace(MoveTemp(MockAction2));

    auto MockAction3 = MakeUnique<FMockBattleAction>();
    CREATE_MOCK_ACTOR(World.Get(), IBattler, Battler3, FMockBattler, MockBattler3);
    auto Battler3Actor = static_cast<AActor*>(Battler3.GetObject());
    auto Battler3Abilities = static_cast<UBattlerAbilityComponent*>(Battler3Actor->AddComponentByClass(UBattlerAbilityComponent::StaticClass(), false, FTransform(), false));
    Battler3Actor->DispatchBeginPlay();
    ON_CALL(MockBattler3, GetAbilityComponent).WillByDefault(Return(Battler3Abilities));
    ON_CALL(*MockAction3, GetPriority).WillByDefault(Return(0));
    ON_CALL(*MockAction3, GetBattler).WillByDefault(ReturnRef(Battler3));
    Battler3Abilities->GetCoreAttributes()->InitSpeed(60);
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
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    CREATE_MOCK_ACTOR(World.Get(), IBattleSide, Side1, FMockBattleSide, MockSide1);
    CREATE_MOCK_ACTOR(World.Get(), IBattleSide, Side2, FMockBattleSide, MockSide2);

    auto Side1Actor = CastChecked<AActor>(Side1.GetObject());
    Side1Actor->AddComponentByClass(UBattleSideAbilitySystemComponent::StaticClass(), false, FTransform(), false);
    auto Side2Actor = CastChecked<AActor>(Side2.GetObject());
    Side2Actor->AddComponentByClass(UBattleSideAbilitySystemComponent::StaticClass(), false, FTransform(), false);

    
    TArray<TScriptInterface<IBattler>> EmptyBattlers;
    ON_CALL(MockSide1, GetBattlers).WillByDefault(ReturnRef(EmptyBattlers));
    ON_CALL(MockSide2, GetBattlers).WillByDefault(ReturnRef(EmptyBattlers));

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

   auto MockAction2 = MakeUnique<FMockBattleAction>();
    ON_CALL(*MockAction2, CanExecute).WillByDefault(Return(true));
    EXPECT_CALL(*MockAction2, IsExecuting).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(*MockAction2, GetActionMessage).WillByDefault(Return(FText::GetEmpty()));

    CREATE_MOCK(IBattler, Target, FMockBattler, MockTarget);
    ON_CALL(*MockAction2, IsComplete).WillByDefault(Return(true));
    auto Action2 = MockAction2.Get();
    ActionQueue.Enqueue(MoveTemp(MockAction2));

    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_ASSERT_TRUE(ActionQueue.Peek()->Get() == Action2);
    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    Battle->TickActor(1, LEVELTICK_All, TickFunction);
    UE_CHECK_TRUE(ActionQueue.IsEmpty());

    ActionQueue.Empty();
    return true;
}