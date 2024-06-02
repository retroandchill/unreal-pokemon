#include "Components/Bag/ItemOption.h"
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Primatives/DisplayText.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemOptionTest, "Unit Tests.UI.Components.ItemOptionTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemOptionTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemOption>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Option = CreateWidget<UItemOption>(World.Get(), WidgetClass);
    Option->AddToViewport();

    Option->SetItem(TEXT("POTION"), 5);

    FIND_CHILD_WIDGET(Option, UDisplayText, ItemNameText);
    ASSERT_NOT_NULL(ItemNameText);
    CHECK_EQUAL(TEXT("Potion"), ItemNameText->GetText().ToString());

    FIND_CHILD_WIDGET(Option, UDisplayText, ItemQuantityText);
    ASSERT_NOT_NULL(ItemQuantityText);
    CHECK_EQUAL(TEXT("×5"), ItemQuantityText->GetText().ToString());

    Option->SetItem(TEXT("OLDROD"), 1);
    CHECK_EQUAL(TEXT("Old Rod"), ItemNameText->GetText().ToString());
    CHECK_EQUAL(TEXT(""), ItemQuantityText->GetText().ToString());

    return true;
}