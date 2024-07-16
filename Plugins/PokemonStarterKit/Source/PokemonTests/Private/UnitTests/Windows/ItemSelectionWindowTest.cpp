#include "Asserts.h"
#include "Bag/Item.h"
#include "Input/UIActionBinding.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/Dispatchers/ItemSlotDispatcher.h"
#include "UtilityClasses/Dispatchers/NoItemSelectedDispatcher.h"
#include "UtilityClasses/Dispatchers/PocketNameDispatcher.h"
#include "Windows/ItemSelectionWindow.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_Basic,
                                 "Unit Tests.Windows.ItemSelectionWindowTest.BasicSelection",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_Basic::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    UE_ASSERT_NULL(ItemSelection->GetCurrentItem());

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("POTION"), 5);
    Bag->ObtainItem(TEXT("FULLHEAL"), 10);
    Bag->ObtainItem(TEXT("BURNHEAL"), 20);

    auto Dispatcher = NewObject<UItemSlotDispatcher>(World.Get());
    ItemSelection->GetOnItemChanged().AddUniqueDynamic(Dispatcher, &UItemSlotDispatcher::ReceiveItem);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    ItemSelection->SetIndex(0);
    UE_CHECK_EQUAL(TEXT("POTION"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(5, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("POTION"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(10, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(2);
    UE_CHECK_EQUAL(TEXT("BURNHEAL"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(20, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("BURNHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_NoItems, "Unit Tests.Windows.ItemSelectionWindowTest.NoItems",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_NoItems::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    UE_ASSERT_NULL(ItemSelection->GetCurrentItem());

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    auto Dispatcher = NewObject<UNoItemSelectedDispatcher>(World.Get());
    ItemSelection->GetOnNoItemSelected().AddUniqueDynamic(Dispatcher, &UNoItemSelectedDispatcher::OnReceive);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    UE_ASSERT_TRUE(Dispatcher->bCalled);

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ItemSelectionWindowTest_Pockets,
                                 "Unit Tests.Windows.ItemSelectionWindowTest.PocketCycling",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool ItemSelectionWindowTest_Pockets::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UItemSelectionWindow>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    TWidgetPtr<UItemSelectionWindow> ItemSelection(CreateWidget<UItemSelectionWindow>(World.Get(), WidgetClass));
    UE_ASSERT_NULL(ItemSelection->GetCurrentItem());
    ItemSelection->AddToViewport();

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("POTION"), 5);
    Bag->ObtainItem(TEXT("FULLHEAL"), 10);

    Bag->ObtainItem(TEXT("REPEL"), 40);
    Bag->ObtainItem(TEXT("SUPERREPEL"), 100);

    auto Dispatcher = NewObject<UPocketNameDispatcher>(World.Get());
    ItemSelection->GetOnPocketChanged().AddUniqueDynamic(Dispatcher, &UPocketNameDispatcher::OnReceivePocket);

    ItemSelection->SetBag(Bag, TEXT("Medicine"));
    UE_CHECK_EQUAL(TEXT("Medicine"), Dispatcher->CurrentPocket.ToString());
    ItemSelection->SetIndex(0);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("POTION"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());

    auto PreviousActionHandle = ItemSelection->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuPrevious"; });
    UE_ASSERT_NOT_NULL(PreviousActionHandle);
    auto PreviousAction = FUIActionBinding::FindBinding(*PreviousActionHandle);
    UE_ASSERT_NOT_NULL(PreviousAction.Get());
    auto NextActionHandle = ItemSelection->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuNext"; });
    UE_ASSERT_NOT_NULL(NextActionHandle);
    auto NextAction = FUIActionBinding::FindBinding(*NextActionHandle);
    UE_ASSERT_NOT_NULL(NextAction.Get());

    ItemSelection->ActivateWidget();
    UE_CHECK_TRUE(PreviousAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(TEXT("Items"), Dispatcher->CurrentPocket.ToString());
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("REPEL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(1);
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("SUPERREPEL"), ItemSelection->GetCurrentItem()->ID.ToString());

    UE_CHECK_TRUE(NextAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(TEXT("Medicine"), Dispatcher->CurrentPocket.ToString());
    UE_ASSERT_NOT_NULL(ItemSelection->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelection->GetCurrentItem()->ID.ToString());
    ItemSelection->SetIndex(0);
    UE_CHECK_TRUE(PreviousAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(TEXT("Items"), Dispatcher->CurrentPocket.ToString());
    UE_CHECK_EQUAL(TEXT("SUPERREPEL"), ItemSelection->GetCurrentItem()->ID.ToString());

    return true;
}