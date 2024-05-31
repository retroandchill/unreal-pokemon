#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Map/GridBasedMap.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridBasedMapDetailsTest, "Unit Tests.GridBasedMapDetailsTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GridBasedMapDetailsTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto &EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.bHideSelectionTip = true;
    auto DetailsView = EditModule.CreateDetailView(DetailsViewArgs);

    auto Character = World->SpawnActor<AGridBasedMap>();
    DetailsView->SetObject(Character);
    auto Categories = UReflectionUtils::GetPropertyCategories(*DetailsView);
    ASSERT_TRUE(Categories.Num() > 6);
    CHECK_EQUAL(TEXT("Transform"), Categories[0]);
    CHECK_EQUAL(TEXT("Tile Map"), Categories[1]);
    CHECK_EQUAL(TEXT("Display"), Categories[2]);
    CHECK_EQUAL(TEXT("Audio"), Categories[3]);
    CHECK_EQUAL(TEXT("Tiles"), Categories[4]);
    CHECK_EQUAL(TEXT("Z-Sorting"), Categories[5]);

    return true;
}
#endif