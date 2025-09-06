#ifdef HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "DependencyInjectionSettings.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestDependencyInjectionSettingsCustomization,
                                 "Unit Tests.DependencyInjection.TestDependencyInjectionSettingsCustomization",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestDependencyInjectionSettingsCustomization::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto &EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
    FDetailsViewArgs DetailsViewArgs;
    DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
    DetailsViewArgs.bHideSelectionTip = true;
    auto DetailsView = EditModule.CreateDetailView(DetailsViewArgs);

    auto Settings = NewObject<UDependencyInjectionSettings>(World.Get());
    DetailsView->SetObject(Settings);
    auto Categories = UReflectionUtils::GetPropertyCategories(*DetailsView);
    UE_ASSERT_TRUE(Categories.Num() > 0);
    UE_CHECK_EQUAL(TEXT("Dependency Injection"), Categories[0]);

    return true;
}
#endif