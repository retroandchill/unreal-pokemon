#include "Asserts.h"
#include "Characters/GridBasedBillboardCharacter.h"
#include "GridBasedGameModeBase.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GridBasedBillboardCharacterDetailsTest,
                                 "Unit Tests.GridBasedBillboardCharacterDetailsTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GridBasedBillboardCharacterDetailsTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld(false);
    auto &EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.bHideSelectionTip = true;
    auto DetailsView = EditModule.CreateDetailView(DetailsViewArgs);

    auto Character = World->SpawnActor<AGridBasedBillboardCharacter>();
    DetailsView->SetObject(Character);
    auto Categories = UReflectionUtils::GetPropertyCategories(*DetailsView);
    UE_ASSERT_TRUE(Categories.Num() > 2);
    UE_CHECK_EQUAL(TEXT("Transform"), Categories[0]);
    UE_CHECK_EQUAL(TEXT("Rendering"), Categories[1]);

    return true;
}