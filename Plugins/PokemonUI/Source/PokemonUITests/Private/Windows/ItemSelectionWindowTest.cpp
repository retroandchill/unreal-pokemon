#include "Asserts.h"
#include "Bag/Item.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/ItemSelectionWindow.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest, "Unit Tests.Windows.ItemSelectionWindowTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    ASSERT_NULL(ItemSelection->GetCurrentItem());

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("POTION"), 5);
    Bag->ObtainItem(TEXT("FULLHEAL"), 10);
    Bag->ObtainItem(TEXT("BURNHEAL"), 20);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    ItemSelection->SetIndex(0);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("POTION"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(2);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("BURNHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());

    return true;
}