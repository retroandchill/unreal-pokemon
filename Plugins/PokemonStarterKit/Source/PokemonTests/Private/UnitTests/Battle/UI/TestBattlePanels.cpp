#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Components/PokemonBattlePanel.h"
#include "Components/PokemonBattlePanelPlayer.h"
#include "Components/ProgressBar.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Primatives/DisplayText.h"
#include "Primatives/NumberImageWidget.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattlePanels, "Private.UnitTests.Battle.UI.TestBattlePanels",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattlePanels::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonBattlePanelPlayer>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Battler, MockBattler] = UnrealMock::CreateMock<IBattler>();
    When(Method(MockBattler, IsFainted)).Return(false, false, true);
    When(Method(MockBattler, GetNickname)).AlwaysReturn(FText::FromStringView(TEXT("Lucario")));
    When(Method(MockBattler, GetPokemonLevel)).AlwaysReturn(50);
    When(Method(MockBattler, GetHP)).Return(100, 50, 0);
    When(Method(MockBattler, GetMaxHP)).AlwaysReturn(100);
    When(Method(MockBattler, GetHPPercent)).Return(1, 0.5f, 0.f);
    When(Method(MockBattler, GetGender)).AlwaysReturn(EPokemonGender::Male);
    When(Method(MockBattler, GetExpPercent)).AlwaysReturn(0.25f);

    TWidgetPtr<UPokemonBattlePanelPlayer> Panel(CreateWidget<UPokemonBattlePanelPlayer>(World.Get(), WidgetClass));
    Panel->AddToViewport();
    Panel->SetBattler(Battler);

    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonName);
    ASSERT_NOT_NULL(PokemonName);
    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonGender);
    ASSERT_NOT_NULL(PokemonGender);
    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonLevel);
    ASSERT_NOT_NULL(PokemonLevel);
    FIND_CHILD_WIDGET(Panel.Get(), UProgressBar, HPBar);
    ASSERT_NOT_NULL(HPBar);
    FIND_CHILD_WIDGET(Panel.Get(), UNumberImageWidget, CurrentHP);
    ASSERT_NOT_NULL(CurrentHP);
    FIND_CHILD_WIDGET(Panel.Get(), UNumberImageWidget, MaxHP);
    ASSERT_NOT_NULL(MaxHP);
    FIND_CHILD_WIDGET(Panel.Get(), UProgressBar, ExpBar);
    ASSERT_NOT_NULL(ExpBar);

    CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, Panel->GetVisibility());
    CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    CHECK_EQUAL(1.f, HPBar->GetPercent());
    CHECK_EQUAL(100, CurrentHP->GetNumber());
    CHECK_EQUAL(100, MaxHP->GetNumber());
    CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    Panel->Refresh();
    CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, Panel->GetVisibility());
    CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    CHECK_EQUAL(0.5f, HPBar->GetPercent());
    CHECK_EQUAL(50, CurrentHP->GetNumber());
    CHECK_EQUAL(100, MaxHP->GetNumber());
    CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    Panel->Refresh();
    CHECK_EQUAL(ESlateVisibility::Hidden, Panel->GetVisibility());
    CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    CHECK_EQUAL(0.0f, HPBar->GetPercent());
    CHECK_EQUAL(0, CurrentHP->GetNumber());
    CHECK_EQUAL(100, MaxHP->GetNumber());
    CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    return true;
}