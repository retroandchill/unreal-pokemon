#include "Asserts.h"
#include "Characters/GridBasedBillboardCharacter.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridBasedBillboardCharacterDetailsTest,
                                 "Private.UnitTests.GridBasedBillboardCharacterDetailsTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GridBasedBillboardCharacterDetailsTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto &EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.bHideSelectionTip = true;
    auto DetailsView = EditModule.CreateDetailView(DetailsViewArgs);

    auto Character = World->SpawnActor<AGridBasedBillboardCharacter>();
    DetailsView->SetObject(Character);
    auto Categories = UReflectionUtils::GetPropertyCategories(*DetailsView);
    ASSERT_TRUE(Categories.Num() > 2);
    CHECK_EQUAL(TEXT("Transform"), Categories[0]);
    CHECK_EQUAL(TEXT("Rendering"), Categories[1]);

    return true;
}