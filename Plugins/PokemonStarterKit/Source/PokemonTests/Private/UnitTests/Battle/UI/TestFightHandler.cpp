#include "Asserts.h"
#include "CommonButtonBase.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Handlers/FightHandler.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Screens/PokemonBattleScreen.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include "Windows/BattleMoveSelect.h"
#include "Windows/PokemonActionOptions.h"
#include <range/v3/view/single.hpp>

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestFightHandler, "Unit Tests.Battle.UI.TestFightHandler",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestFightHandler::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonBattleScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UPokemonBattleScreen> Screen(CreateWidget<UPokemonBattleScreen>(World.Get(), WidgetClass));
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UPokemonActionOptions, ActionSelect);
    UE_ASSERT_NOT_NULL(ActionSelect);
    FIND_CHILD_WIDGET(Screen.Get(), UBattleMoveSelect, MoveSelect);
    UE_ASSERT_NOT_NULL(MoveSelect);

    CREATE_MOCK(IBattle, Battle, FMockBattle, MockBattle);
    CREATE_MOCK(IBattleSide, Side, FMockBattleSide, MockSide);
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(ReturnRef(Battle));
    ON_CALL(MockSide, ShowBackSprites).WillByDefault(Return(false));
    ON_CALL(MockBattle, GetSides).WillByDefault(Return(ranges::views::single(Side)));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{.Species = TEXT("MIMIKYU"),
                    .Level = 50,
                    .Moves = {TEXT("SHADOWSNEAK"), TEXT("PLAYROUGH"), TEXT("SWORDSDANCE"), TEXT("SHADOWCLAW")}});
    auto Battler1 = World->SpawnActor<ATestBattlerActor>();
    Battler1->DispatchBeginPlay();
    Battler1->Initialize(Side, Pokemon1);

    auto Pokemon2 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{.Species = TEXT("LUCARIO"),
                    .Level = 50,
                    .Moves = {TEXT("AURASPHERE"), TEXT("DRAGONPULSE"), TEXT("FLASHCANNON"), TEXT("NASTYPLOT")}});
    auto Battler2 = World->SpawnActor<ATestBattlerActor>();
    Battler2->DispatchBeginPlay();
    Battler2->Initialize(Side, Pokemon2);

    TArray<TScriptInterface<IBattler>> Battlers = {Battler1, Battler2};
    ON_CALL(MockSide, GetBattlers).WillByDefault(ReturnRef(Battlers));
    Screen->SetBattle(Battle);

    Screen->SelectAction(Battler1);
    Screen->SelectAction(Battler2);
    UE_CHECK_EQUAL(ESlateVisibility::Visible, ActionSelect->GetVisibility());

    auto &Options = UReflectionUtils::GetMutablePropertyValue<TArray<TObjectPtr<UBattleMenuHandler>>>(ActionSelect, "MenuActions");
    int32 FightHandlerIndex =
        Options.IndexOfByPredicate([](UBattleMenuHandler *Handler) { return Handler->IsA<UFightHandler>(); });
    ActionSelect->SetIndex(FightHandlerIndex);
    ActionSelect->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Visible, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, ActionSelect->GetVisibility());

    MoveSelect->GetOnCancel().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Visible, ActionSelect->GetVisibility());

    ActionSelect->SetIndex(FightHandlerIndex);
    ActionSelect->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Visible, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, ActionSelect->GetVisibility());

    MoveSelect->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Visible, ActionSelect->GetVisibility());

    ActionSelect->SetIndex(FightHandlerIndex);
    ActionSelect->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Visible, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, ActionSelect->GetVisibility());

    MoveSelect->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, MoveSelect->GetVisibility());
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, ActionSelect->GetVisibility());

    return true;
}