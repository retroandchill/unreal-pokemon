#include "Asserts.h"
#include "Battle/Actions/BattleAction.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/PokemonBattle.h"
#include "External/accessor.hpp"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestPokemonBattle.h"

using namespace fakeit;
using namespace accessor;

MEMBER_ACCESSOR(AccessActionsQueueing, APokemonBattle, SelectedActions, TArray<TUniquePtr<IBattleAction>>)
MEMBER_ACCESSOR(AccessActionQueueQueueing, APokemonBattle, ActionQueue, TQueue<TUniquePtr<IBattleAction>>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestActionQueueing, "Unit Tests.Battle.TestActionQueueing",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestActionQueueing::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto [Side1, MockSide1] = UnrealMock::CreateMock<IBattleSide>();
    auto [Side2, MockSide2] = UnrealMock::CreateMock<IBattleSide>();

    auto Battle = World->SpawnActor<ATestPokemonBattle>();
    Battle->Initialize({Side1, Side2});

    TArray<TUniquePtr<Mock<IBattleAction>>> Actions;
    auto QueueBattleAction = [&Actions, &Battle](const TScriptInterface<IBattler> &Battler) {
        auto &MockAction = Actions.Emplace_GetRef(MakeUnique<Mock<IBattleAction>>());
        When(Method(*MockAction, GetBattler)).AlwaysReturn(Battler);
        Battle->QueueAction(TUniquePtr<IBattleAction>(&MockAction->get()));
    };

    TArray<TScriptInterface<IBattler>> Side1Battlers;
    auto [Battler1, MockBattler1] = UnrealMock::CreateMock<IBattler>();
    auto Battler1ID = FGuid::NewGuid();
    When(Method(MockBattler1, GetInternalId)).AlwaysReturn(Battler1ID);
    When(Method(MockBattler1, SelectActions)).Do(std::bind_front(QueueBattleAction, Battler1));
    When(Method(MockBattler1, GetActionCount)).AlwaysReturn(1);
    When(Method(MockBattler1, IsFainted)).AlwaysReturn(false);
    When(Method(MockBattler1, GetNickname)).AlwaysReturn(FText::FromStringView(TEXT("Battler 1")));
    Side1Battlers.Add(Battler1);
    auto [Battler2, MockBattler2] = UnrealMock::CreateMock<IBattler>();
    auto Battler2ID = FGuid::NewGuid();
    When(Method(MockBattler2, GetInternalId)).AlwaysReturn(Battler2ID);
    When(Method(MockBattler2, SelectActions)).Do(std::bind_front(QueueBattleAction, Battler2));
    When(Method(MockBattler2, GetActionCount)).AlwaysReturn(1);
    When(Method(MockBattler2, IsFainted)).AlwaysReturn(false);
    When(Method(MockBattler2, GetNickname)).AlwaysReturn(FText::FromStringView(TEXT("Battler 2")));
    Side1Battlers.Add(Battler2);
    When(Method(MockSide1, GetBattlers)).AlwaysReturn(Side1Battlers);

    TArray<TScriptInterface<IBattler>> Side2Battlers;
    auto [Battler3, MockBattler3] = UnrealMock::CreateMock<IBattler>();
    auto Battler3ID = FGuid::NewGuid();
    When(Method(MockBattler3, GetInternalId)).AlwaysReturn(Battler3ID);
    When(Method(MockBattler3, SelectActions)).Do(std::bind_front(QueueBattleAction, Battler3));
    When(Method(MockBattler3, GetActionCount)).AlwaysReturn(2);
    When(Method(MockBattler3, IsFainted)).AlwaysReturn(false);
    When(Method(MockBattler3, GetNickname)).AlwaysReturn(FText::FromStringView(TEXT("Battler 3")));
    Side2Battlers.Add(Battler3);
    When(Method(MockSide2, GetBattlers)).AlwaysReturn(Side2Battlers);

    Battle->StartBattle();
    ASSERT_FALSE(Battle->ActionSelectionFinished());
    AddExpectedError(TEXT("Battler 1 attempted to queue an action, but is already at capacity!"));
    QueueBattleAction(Battler1);
    ASSERT_FALSE(Battle->ActionSelectionFinished());
    QueueBattleAction(Battler3);
    ASSERT_TRUE(Battle->ActionSelectionFinished());

    accessMember<AccessActionsQueueing>(*Battle).get().Empty();
    accessMember<AccessActionQueueQueueing>(*Battle).get().Empty();
    return true;
}