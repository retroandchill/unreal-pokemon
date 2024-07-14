
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/SelectableWidget.h"

constexpr auto TEST_SELECTABLE = TEXT("/RPGMenus/Tests/Resources/TestSelectable.TestSelectable");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SelectableWidgetInputTest, "Unit Tests.RPGMenus.SelectableWidgetInputTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SelectableWidgetInputTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto WidgetClass = UBlueprintTestUtils::LoadBlueprintClassByName(TEST_SELECTABLE);
    UE_ASSERT_NOT_NULL(WidgetClass);

    auto NewWidget = CreateWidget<USelectableWidget>(World.Get(), WidgetClass);
    UE_ASSERT_NOT_NULL(NewWidget);
    NewWidget->AddToViewport();

    NewWidget->ActivateWidget();
    NewWidget->SetIndex(0);

    UE_ASSERT_EQUAL(6, NewWidget->GetItemCount());

    auto &UpKey = EKeys::W;
    auto &LeftKey = EKeys::A;
    auto &DownKey = EKeys::S;
    auto &RightKey = EKeys::D;
    auto &ConfirmKey = EKeys::Enter;
    auto &CancelKey = EKeys::BackSpace;

    UInputUtilities::SimulateKeyPress(NewWidget, DownKey);
    UE_ASSERT_EQUAL(2, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    UE_ASSERT_EQUAL(0, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, UpKey);
    UE_ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    UE_ASSERT_EQUAL(3, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UE_ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UE_ASSERT_EQUAL(5, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    UE_ASSERT_EQUAL(4, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UInputUtilities::SimulateKeyPress(NewWidget, RightKey);
    UE_ASSERT_EQUAL(0, NewWidget->GetIndex());

    UInputUtilities::SimulateKeyPress(NewWidget, LeftKey);
    UE_ASSERT_EQUAL(5, NewWidget->GetIndex());

    NewWidget->DeactivateWidget();
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    auto IndexSet = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("ConfirmCalled"));
    UE_ASSERT_FALSE(IndexSet);

    NewWidget->ActivateWidget();
    UInputUtilities::SimulateKeyPress(NewWidget, ConfirmKey);
    IndexSet = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("ConfirmCalled"));
    UE_ASSERT_TRUE(IndexSet);
    auto CalledIndex = UReflectionUtils::GetPropertyValue<int32>(NewWidget, TEXT("CalledIndex"));
    UE_ASSERT_EQUAL(5, CalledIndex);

    NewWidget->ActivateWidget();
    UE_ASSERT_TRUE(NewWidget->IsActivated());

    UInputUtilities::SimulateKeyPress(NewWidget, CancelKey);
    auto bCanceled = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("CancelCalled"));
    UE_ASSERT_TRUE(bCanceled);

    NewWidget->Deselect();
    UE_ASSERT_EQUAL(-1, NewWidget->GetIndex());

    NewWidget->Deselect();
    UE_ASSERT_EQUAL(-1, NewWidget->GetIndex());

    return true;
}