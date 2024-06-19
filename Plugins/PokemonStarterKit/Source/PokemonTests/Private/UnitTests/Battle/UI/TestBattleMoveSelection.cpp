#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Components/BattleMovePanel.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattle.h"
#include "Mocks/MockBattleSide.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Primatives/DisplayText.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/BattleActors/TestBattlerActor.h"
#include "Windows/BattleMoveSelect.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattleMoveSelection, "Unit Tests.Battle.UI.TestBattleMoveSelection",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattleMoveSelection::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UBattleMoveSelect>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle, FMockBattle>();
    auto [Side, MockSide] = UnrealMock::CreateMock<IBattleSide, FMockBattleSide>();
    ON_CALL(MockSide, GetOwningBattle).WillByDefault(Return(Battle));
    ON_CALL(MockSide, ShowBackSprites).WillByDefault(Return(false));

    auto Pokemon = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(),
        FPokemonDTO{.Species = TEXT("MIMIKYU"),
                    .Level = 50,
                    .Moves = {TEXT("SHADOWSNEAK"), TEXT("PLAYROUGH"), TEXT("SWORDSDANCE"), TEXT("SHADOWCLAW")}});
    auto Battler = World->SpawnActor<ATestBattlerActor>();
    Battler->Initialize(Side, Pokemon);

    TWidgetPtr<UBattleMoveSelect> BattleMoveSelect(CreateWidget<UBattleMoveSelect>(World.Get(), WidgetClass));
    BattleMoveSelect->AddToViewport();
    BattleMoveSelect->SetBattler(Battler);

    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel0);
    UE_ASSERT_NOT_NULL(MovePanel0);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel1);
    UE_ASSERT_NOT_NULL(MovePanel1);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel2);
    UE_ASSERT_NOT_NULL(MovePanel2);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel3);
    UE_ASSERT_NOT_NULL(MovePanel3);

    FIND_CHILD_WIDGET(MovePanel0, UDisplayText, MoveNameText);
    UE_ASSERT_NOT_NULL(MoveNameText);
    FIND_CHILD_WIDGET(MovePanel0, UDisplayText, MovePPText);
    UE_ASSERT_NOT_NULL(MovePPText);

    UE_CHECK_EQUAL(TEXT("Shadow Sneak"), MoveNameText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("30/30"), MovePPText->GetText().ToString());

    return true;
}