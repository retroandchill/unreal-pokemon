
#include "Actions/MenuAction.h"
#include "Asserts.h"
#include "PrimaryGameLayout.h"
#include "Misc/AutomationTest.h"
#include "Screens/Screen.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/WidgetTestUtilities.h"

constexpr auto TEST_MENU_ACTION = TEXT("/PokemonStarterKit/Tests/Resources/TestMenuAction.TestMenuAction");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MenuActionTest, "Unit Tests.UI.MenuActionTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MenuActionTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto MenuActionClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_MENU_ACTION);
    UE_ASSERT_NOT_NULL(MenuActionClass);

    auto MenuAction = NewObject<UMenuAction>(World.Get(), MenuActionClass);
    UE_ASSERT_NOT_NULL(MenuAction);
    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    MenuAction->PerformAction(Pawn);
    auto Layer = UPrimaryGameLayout::GetPrimaryGameLayout(Player.Get())->GetLayerWidget(RPG::Menus::PrimaryMenuLayerTag);
    auto TopOfStack = Layer->GetActiveWidget();
    UE_ASSERT_NOT_NULL(TopOfStack);

    return true;
}