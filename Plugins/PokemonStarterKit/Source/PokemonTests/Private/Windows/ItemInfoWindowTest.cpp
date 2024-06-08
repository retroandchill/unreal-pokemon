﻿#include "Windows/ItemInfoWindow.h"
#include "Asserts.h"
#include "Bag/Item.h"
#include "Components/Image.h"
#include "DataManager.h"
#include "Misc/AutomationTest.h"
#include "Primatives/DisplayText.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemInfoWindowTest, "Unit Tests.Windows.ItemInfoWindowTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemInfoWindowTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemInfoWindow>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemInfoWindow> ItemInfo(CreateWidget<UItemInfoWindow>(World.Get(), WidgetClass));
    FIND_CHILD_WIDGET(ItemInfo.Get(), UDisplayText, ItemDescription);
    ASSERT_NOT_NULL(ItemDescription);
    FIND_CHILD_WIDGET(ItemInfo.Get(), UImage, ItemIcon);
    ASSERT_NOT_NULL(ItemIcon);

    auto ExistingItem = FDataManager::GetInstance().GetDataTable<FItem>().GetData(TEXT("POTION"));
    ASSERT_NOT_NULL(ExistingItem);
    ItemInfo->Refresh(*ExistingItem, 10);
    CHECK_EQUAL(ExistingItem->Description.ToString(), ItemDescription->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::SelfHitTestInvisible, ItemIcon->GetVisibility());

    ItemInfo->ClearItem();
    CHECK_EQUAL(TEXT(""), ItemDescription->GetText().ToString());
    CHECK_EQUAL(ESlateVisibility::Hidden, ItemIcon->GetVisibility());

    return true;
}