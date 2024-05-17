#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Actions/MenuAction.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "RPGPlayerController.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/WidgetTestUtilities.h"

constexpr auto TEST_MENU_ACTION = TEXT("/PokemonUI/Tests/Resources/TestMenuAction.TestMenuAction");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MenuActionTest, "Unit Tests.UI.MenuActionTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool MenuActionTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();
    auto MenuActionClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_MENU_ACTION);
    ASSERT_NOT_NULL(MenuActionClass);

    auto MenuAction = NewObject<UMenuAction>(World, MenuActionClass);
    ASSERT_NOT_NULL(MenuAction);
    auto PlayerController = World->SpawnActor<ARPGPlayerController>();
    auto Pawn = World->SpawnActor<APawn>();
    auto Player = NewObject<ULocalPlayer>(GEngine);
    PlayerController->Possess(Pawn);
    PlayerController->Player = Player;
    MenuAction->PerformAction(Pawn);
    auto TopOfStack = PlayerController->GetTopOfStack();
    ASSERT_NOT_NULL(TopOfStack);

    return true;
}
#endif