#include "Components/Bag/ItemOption.h"
#include "Asserts.h"
#include "CommonTextBlock.h"
#include "Misc/AutomationTest.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemOptionTest, "Unit Tests.UI.Components.ItemOptionTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemOptionTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemOption>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Option = CreateWidget<UItemOption>(World.Get(), WidgetClass);
    Option->AddToViewport();

    Option->SetItem(TEXT("POTION"), 5);

    FIND_CHILD_WIDGET(Option, UCommonTextBlock, ItemNameText);
    UE_ASSERT_NOT_NULL(ItemNameText);
    UE_CHECK_EQUAL(TEXT("Potion"), ItemNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Option, UCommonTextBlock, ItemQuantityText);
    UE_ASSERT_NOT_NULL(ItemQuantityText);
    UE_CHECK_EQUAL(TEXT("×5"), ItemQuantityText->GetText().ToString());

    Option->SetItem(TEXT("OLDROD"), 1);
    UE_CHECK_EQUAL(TEXT("Old Rod"), ItemNameText->GetText().ToString());
    UE_CHECK_EQUAL(TEXT(""), ItemQuantityText->GetText().ToString());

    return true;
}