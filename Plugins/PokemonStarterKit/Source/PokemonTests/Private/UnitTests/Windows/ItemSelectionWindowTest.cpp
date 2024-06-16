﻿#include "Asserts.h"
#include "Bag/Item.h"
#include "Data/SelectionInputs.h"
#include "External/accessor.hpp"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/Dispatchers/ItemSlotDispatcher.h"
#include "UtilityClasses/Dispatchers/NoItemSelectedDispatcher.h"
#include "UtilityClasses/Dispatchers/PocketNameDispatcher.h"
#include "Windows/ItemSelectionWindow.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessInputMappings, USelectableWidget, InputMappings, TObjectPtr<USelectionInputs>)
MEMBER_ACCESSOR(AccessLeftInputs, USelectionInputs, LeftInputs, TSet<FKey>)
MEMBER_ACCESSOR(AccessRightInputs, USelectionInputs, RightInputs, TSet<FKey>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_Basic,
                                 "Unit Tests.Windows.ItemSelectionWindowTest.BasicSelection",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_Basic::RunTest(const FString &Parameters) {
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

    auto Dispatcher = NewObject<UItemSlotDispatcher>(World.Get());
    ItemSelection->GetOnItemChanged().AddDynamic(Dispatcher, &UItemSlotDispatcher::ReceiveItem);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    ItemSelection->SetIndex(0);
    CHECK_EQUAL(TEXT("POTION"), Dispatcher->ItemID.ToString());
    CHECK_EQUAL(5, Dispatcher->Quantity);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("POTION"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    CHECK_EQUAL(TEXT("FULLHEAL"), Dispatcher->ItemID.ToString());
    CHECK_EQUAL(10, Dispatcher->Quantity);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(2);
    CHECK_EQUAL(TEXT("BURNHEAL"), Dispatcher->ItemID.ToString());
    CHECK_EQUAL(20, Dispatcher->Quantity);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("BURNHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_NoItems,
                                 "Unit Tests.Windows.ItemSelectionWindowTest.NoItems",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_NoItems::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    ASSERT_NULL(ItemSelection->GetCurrentItem());

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    auto Dispatcher = NewObject<UNoItemSelectedDispatcher>(World.Get());
    ItemSelection->GetOnNoItemSelected().AddDynamic(Dispatcher, &UNoItemSelectedDispatcher::OnReceive);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    ASSERT_TRUE(Dispatcher->bCalled);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_Pockets,
                                 "Unit Tests.Windows.ItemSelectionWindowTest.PocketCycling",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_Pockets::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    ASSERT_NULL(ItemSelection->GetCurrentItem());

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("POTION"), 5);
    Bag->ObtainItem(TEXT("FULLHEAL"), 10);

    Bag->ObtainItem(TEXT("REPEL"), 40);
    Bag->ObtainItem(TEXT("SUPERREPEL"), 100);

    auto Dispatcher = NewObject<UPocketNameDispatcher>(World.Get());
    ItemSelection->GetOnPocketChanged().AddDynamic(Dispatcher, &UPocketNameDispatcher::OnReceivePocket);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    CHECK_EQUAL(TEXT("Medicine"), Dispatcher->CurrentPocket.ToString());
    ItemSelection->SetIndex(0);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("POTION"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());

    USelectionInputs *InputMappings = accessMember<AccessInputMappings>(*ItemSelection).get();
    ASSERT_NOT_NULL(InputMappings);
    auto LeftInput = *accessMember<AccessLeftInputs>(*InputMappings).get().CreateIterator();
    auto RightInput = *accessMember<AccessRightInputs>(*InputMappings).get().CreateIterator();

    ItemSelection->SetActive(true);
    UInputUtilities::SimulateKeyPress(ItemSelection.Get(), LeftInput);
    CHECK_EQUAL(TEXT("Items"), Dispatcher->CurrentPocket.ToString());
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("REPEL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("SUPERREPEL"), ItemSelection->GetCurrentItem()->ID.ToString());

    UInputUtilities::SimulateKeyPress(ItemSelection.Get(), RightInput);
    CHECK_EQUAL(TEXT("Medicine"), Dispatcher->CurrentPocket.ToString());
    ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(0);
    UInputUtilities::SimulateKeyPress(ItemSelection.Get(), LeftInput);
    CHECK_EQUAL(TEXT("Items"), Dispatcher->CurrentPocket.ToString());
    CHECK_EQUAL(TEXT("SUPERREPEL"), ItemSelection->GetCurrentItem()->ID.ToString());

    return true;
}