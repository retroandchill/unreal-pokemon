#include "Screens/BagScreen.h"
#include "Asserts.h"
#include "CommonButtonBase.h"
#include "EnhancedInputSubsystems.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Components/CommandWindow.h"
#include "Components/Bag/ItemSelectionWindow.h"
#include "Components/Bag/PocketTabWidget.h"
#include "Components/Bag/PocketWindow.h"
#include "Input/UIActionBinding.h"
#include "UtilityClasses/Dispatchers/ItemSlotDispatcher.h"
#include "UtilityClasses/Dispatchers/NoItemSelectedDispatcher.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagScreenTest, "Unit Tests.Screens.BagScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagScreenTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UBagScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("POTION"), 5);
    Bag->ObtainItem(TEXT("FULLHEAL"), 10);
    Bag->ObtainItem(TEXT("BURNHEAL"), 20);
    auto &Subsystem = UPokemonSubsystem::GetInstance(World.Get());
    UReflectionUtils::SetPropertyValue<TScriptInterface<IBag>>(&Subsystem, "Bag", Bag);

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    TWidgetPtr<UBagScreen> Screen(CreateWidget<UBagScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UItemSelectionWindow, ItemSelectionWindow);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UPocketTabWidget, PocketTabWidget);
    UE_ASSERT_NOT_NULL(PocketTabWidget);
    FIND_CHILD_WIDGET(Screen.Get(), UPocketWindow, PocketWindow);
    UE_ASSERT_NOT_NULL(PocketWindow);

    UE_ASSERT_EQUAL(TEXT("Items"), PocketWindow->GetCurrentPocket().ToString());

    auto PreviousActionHandle = PocketTabWidget->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuPrevious"; });
    UE_ASSERT_NOT_NULL(PreviousActionHandle);
    auto PreviousAction = FUIActionBinding::FindBinding(*PreviousActionHandle);
    UE_ASSERT_NOT_NULL(PreviousAction.Get());
    auto NextActionHandle = PocketTabWidget->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuNext"; });
    UE_ASSERT_NOT_NULL(NextActionHandle);
    auto NextAction = FUIActionBinding::FindBinding(*NextActionHandle);
    UE_ASSERT_NOT_NULL(NextAction.Get());
    
    UE_CHECK_TRUE(NextAction->OnExecuteAction.ExecuteIfBound());
    UE_ASSERT_EQUAL(TEXT("Medicine"), PocketWindow->GetCurrentPocket().ToString());

    auto Dispatcher = NewObject<UItemSlotDispatcher>(World.Get());
    ItemSelectionWindow->GetOnItemChanged().AddUniqueDynamic(Dispatcher, &UItemSlotDispatcher::ReceiveItem);

    ItemSelectionWindow->SetIndex(0);
    UE_CHECK_EQUAL(TEXT("POTION"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(5, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("POTION"), ItemSelectionWindow->GetCurrentItem()->ID.ToString());
    ItemSelectionWindow->SetIndex(1);
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(10, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("FULLHEAL"), ItemSelectionWindow->GetCurrentItem()->ID.ToString());
    ItemSelectionWindow->SetIndex(2);
    UE_CHECK_EQUAL(TEXT("BURNHEAL"), Dispatcher->ItemID.ToString());
    UE_CHECK_EQUAL(20, Dispatcher->Quantity);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow->GetCurrentItem());
    UE_CHECK_EQUAL(TEXT("BURNHEAL"), ItemSelectionWindow->GetCurrentItem()->ID.ToString());

    auto Dispatcher2 = NewObject<UNoItemSelectedDispatcher>(World.Get());
    ItemSelectionWindow->GetOnNoItemSelected().AddUniqueDynamic(Dispatcher2, &UNoItemSelectedDispatcher::OnReceive);
    
    UE_CHECK_TRUE(PreviousAction->OnExecuteAction.ExecuteIfBound());
    UE_ASSERT_EQUAL(TEXT("Items"), PocketWindow->GetCurrentPocket().ToString());
    UE_ASSERT_TRUE(Dispatcher2->bCalled);

    return true;
}