#include "Asserts.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Components/BattleMovePanel.h"
#include "External/accessor.hpp"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Primatives/DisplayText.h"
#include "TestBattlerActor.h"
#include "TestSpriteActor.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/BattleMoveSelect.h"

using namespace fakeit;
using namespace accessor;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattleMoveSelection, "Unit Tests.Battle.UI.TestBattleMoveSelection",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattleMoveSelection::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UBattleMoveSelect>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Battle, MockBattle] = UnrealMock::CreateMock<IBattle>();
    auto [Side, MockSide] = UnrealMock::CreateMock<IBattleSide>();
    When(Method(MockSide, GetOwningBattle)).AlwaysReturn(Battle);
    When(Method(MockSide, ShowBackSprites)).AlwaysReturn(false);

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
    ASSERT_NOT_NULL(MovePanel0);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel1);
    ASSERT_NOT_NULL(MovePanel1);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel2);
    ASSERT_NOT_NULL(MovePanel2);
    FIND_CHILD_WIDGET(BattleMoveSelect.Get(), UBattleMovePanel, MovePanel3);
    ASSERT_NOT_NULL(MovePanel3);

    FIND_CHILD_WIDGET(MovePanel0, UDisplayText, MoveNameText);
    ASSERT_NOT_NULL(MoveNameText);
    FIND_CHILD_WIDGET(MovePanel0, UDisplayText, MovePPText);
    ASSERT_NOT_NULL(MovePPText);

    CHECK_EQUAL(TEXT("Shadow Sneak"), MoveNameText->GetText().ToString());
    CHECK_EQUAL(TEXT("30/30"), MovePPText->GetText().ToString());

    return true;
}