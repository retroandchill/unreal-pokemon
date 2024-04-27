#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Misc/AutomationTest.h"
#include "Asserts.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/SelectableWidget.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/ReflectionUtils.h"

class USelectableWidget;
constexpr auto BLUEPRINT_CLASS_NAME = TEXT("/RPGMenus/Tests/Resources/TestSelectable.TestSelectable");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SelectableWidgetInputTest, "UnrealPokemon.RPGMenus.SelectableWidgetInputTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SelectableWidgetInputTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld(); 
    auto WidgetClass = UBlueprintTestUtils::LoadBlueprintClassByName(BLUEPRINT_CLASS_NAME);    
    ASSERT_NOT_NULL(WidgetClass);

    auto NewWidget = CreateWidget<USelectableWidget>(World, WidgetClass);
    ASSERT_NOT_NULL(NewWidget);
    NewWidget->AddToViewport();

    NewWidget->SetActive(true);
    NewWidget->SetIndex(0);

    ASSERT_EQUAL(6, NewWidget->GetItemCount());
    ASSERT_EQUAL(3, NewWidget->GetRowCount());
    ASSERT_EQUAL(2, NewWidget->GetColumnCount());
    ASSERT_EQUAL(1, NewWidget->GetColumn(4));
    ASSERT_EQUAL(1, NewWidget->GetRow(4));
    ASSERT_EQUAL(2, NewWidget->GetColumn(5));
    ASSERT_EQUAL(1, NewWidget->GetRow(5));

    auto &UpKey = EKeys::W;
    auto &LeftKey = EKeys::A;
    auto &DownKey = EKeys::S;
    auto &RightKey = EKeys::D;
    auto &ConfirmKey = EKeys::Enter;
    auto &CancelKey = EKeys::BackSpace;

    UInputUtilities::SimulateKeyPress(NewWidget, DownKey);
    ASSERT_EQUAL(2, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    ASSERT_EQUAL(0, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    ASSERT_EQUAL(3, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    ASSERT_EQUAL(5, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    ASSERT_EQUAL(0, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    ASSERT_EQUAL(5, NewWidget->GetIndex());

    NewWidget->SetActive(false);
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    auto IndexSet = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("ConfirmCalled"));
    ASSERT_FALSE(IndexSet);

    NewWidget->SetActive(true);
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    IndexSet = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("ConfirmCalled"));
    ASSERT_TRUE(IndexSet);
    auto CalledIndex = UReflectionUtils::GetPropertyValue<int32>(NewWidget, TEXT("CalledIndex"));
    ASSERT_EQUAL(5, CalledIndex);

    NewWidget->SetActive(true);
    ASSERT_TRUE(NewWidget->IsActive());

    UInputUtilities::SimulateKeyPress(NewWidget, CancelKey);
    auto bCanceled = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("CancelCalled"));
    ASSERT_TRUE(bCanceled);

    NewWidget->Deselect();
    ASSERT_EQUAL(-1, NewWidget->GetIndex());

    NewWidget->Deselect();
    ASSERT_EQUAL(-1, NewWidget->GetIndex());

    return true;
}
#endif