

#include "Asserts.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Misc/AutomationTest.h"
#include "Primatives/SelectableOption.h"
#include "Utilities/BlueprintTestUtils.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/SelectableWidget.h"

constexpr auto OPTION_TEST_WIDGET = TEXT("/RPGMenus/Tests/Resources/OptionTestWidget.OptionTestWidget");

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SelectableWidgetMouseTest, "Private.UnitTests.RPGMenus.SelectableWidgetMouseTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SelectableWidgetMouseTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto WidgetClass = UBlueprintTestUtils::LoadBlueprintClassByName(OPTION_TEST_WIDGET);
    ASSERT_NOT_NULL(WidgetClass);
    auto NewWidget = CreateWidget<USelectableWidget>(World.Get(), WidgetClass);
    ASSERT_NOT_NULL(NewWidget);
    NewWidget->AddToViewport();

    auto &Options = UReflectionUtils::GetPropertyValue<TArray<USelectableOption *>>(NewWidget, TEXT("Options"));
    ASSERT_EQUAL(3, Options.Num());

    auto Button1 = Cast<UButton>(UWidgetTestUtilities::FindChildWidget(Options[1], TEXT("PrimaryButton")));
    ASSERT_NOT_NULL(Button1);
    Button1->OnHovered.Broadcast();
    ASSERT_EQUAL(-1, NewWidget->GetIndex());

    NewWidget->SetActive(true);
    Button1->OnHovered.Broadcast();
    ASSERT_EQUAL(1, NewWidget->GetIndex());

    auto Button2 = Cast<UButton>(UWidgetTestUtilities::FindChildWidget(Options[2], TEXT("PrimaryButton")));
    ASSERT_NOT_NULL(Button2);

    NewWidget->SetActive(false);
    Button2->OnClicked.Broadcast();
    auto Clicked = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("Clicked"));
    ASSERT_FALSE(Clicked);

    NewWidget->SetActive(true);
    Button2->OnClicked.Broadcast();
    Clicked = UReflectionUtils::GetPropertyValue<bool>(NewWidget, TEXT("Clicked"));
    ASSERT_TRUE(Clicked);
    auto BroadcastIndex = UReflectionUtils::GetPropertyValue<int32>(NewWidget, TEXT("BroadcastIndex"));
    ASSERT_EQUAL(2, BroadcastIndex);

    return true;
}