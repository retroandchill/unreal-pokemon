#include "Asserts.h"
#include "Battle/Battlers/Battler.h"
#include "Components/PokemonBattlePanel.h"
#include "Components/PokemonBattlePanelPlayer.h"
#include "Components/ProgressBar.h"
#include "Misc/AutomationTest.h"
#include "Mocking/UnrealMock.h"
#include "Mocks/MockBattler.h"
#include "Primatives/DisplayText.h"
#include "Primatives/NumberImageWidget.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

using namespace testing;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestBattlePanels, "Unit Tests.Battle.UI.TestBattlePanels",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestBattlePanels::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonBattlePanelPlayer>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Battler, MockBattler] = UnrealMock::CreateMock<IBattler, FMockBattler>();
    EXPECT_CALL(MockBattler, IsFainted).WillOnce(Return(false)).WillOnce(Return(false)).WillRepeatedly(Return(true));
    ON_CALL(MockBattler, GetNickname).WillByDefault(Return(FText::FromStringView(TEXT("Lucario"))));
    ON_CALL(MockBattler, GetPokemonLevel).WillByDefault(Return(50));
    ON_CALL(MockBattler, GetHP).Return(100, 50, 0);
    ON_CALL(MockBattler, GetMaxHP).WillByDefault(Return(100));
    ON_CALL(MockBattler, GetHPPercent).Return(1, 0.5f, 0.f);
    ON_CALL(MockBattler, GetGender).WillByDefault(Return(EPokemonGender::Male));
    ON_CALL(MockBattler, GetExpPercent).WillByDefault(Return(0.25f));

    TWidgetPtr<UPokemonBattlePanelPlayer> Panel(CreateWidget<UPokemonBattlePanelPlayer>(World.Get(), WidgetClass));
    Panel->AddToViewport();
    Panel->SetBattler(Battler);

    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonName);
    UE_ASSERT_NOT_NULL(PokemonName);
    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonGender);
    UE_ASSERT_NOT_NULL(PokemonGender);
    FIND_CHILD_WIDGET(Panel.Get(), UDisplayText, PokemonLevel);
    UE_ASSERT_NOT_NULL(PokemonLevel);
    FIND_CHILD_WIDGET(Panel.Get(), UProgressBar, HPBar);
    UE_ASSERT_NOT_NULL(HPBar);
    FIND_CHILD_WIDGET(Panel.Get(), UNumberImageWidget, CurrentHP);
    UE_ASSERT_NOT_NULL(CurrentHP);
    FIND_CHILD_WIDGET(Panel.Get(), UNumberImageWidget, MaxHP);
    UE_ASSERT_NOT_NULL(MaxHP);
    FIND_CHILD_WIDGET(Panel.Get(), UProgressBar, ExpBar);
    UE_ASSERT_NOT_NULL(ExpBar);

    UE_CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, Panel->GetVisibility());
    UE_CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    UE_CHECK_EQUAL(1.f, HPBar->GetPercent());
    UE_CHECK_EQUAL(100, CurrentHP->GetNumber());
    UE_CHECK_EQUAL(100, MaxHP->GetNumber());
    UE_CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    Panel->Refresh();
    UE_CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, Panel->GetVisibility());
    UE_CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    UE_CHECK_EQUAL(0.5f, HPBar->GetPercent());
    UE_CHECK_EQUAL(50, CurrentHP->GetNumber());
    UE_CHECK_EQUAL(100, MaxHP->GetNumber());
    UE_CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    Panel->Refresh();
    UE_CHECK_EQUAL(ESlateVisibility::Hidden, Panel->GetVisibility());
    UE_CHECK_EQUAL(TEXT("Lucario"), PokemonName->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("♂"), PokemonGender->GetText().ToString());
    UE_CHECK_EQUAL(TEXT("50"), PokemonLevel->GetText().ToString());
    UE_CHECK_EQUAL(0.0f, HPBar->GetPercent());
    UE_CHECK_EQUAL(0, CurrentHP->GetNumber());
    UE_CHECK_EQUAL(100, MaxHP->GetNumber());
    UE_CHECK_EQUAL(0.25f, ExpBar->GetPercent());

    return true;
}