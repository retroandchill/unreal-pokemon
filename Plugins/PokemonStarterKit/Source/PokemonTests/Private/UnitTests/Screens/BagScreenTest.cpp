#include "Screens/BagScreen.h"
#include "Asserts.h"
#include "Data/SelectionInputs.h"
#include "Handlers/BagMenu/BagMenuHandlerSet.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "RPGMenusSubsystem.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/Dispatchers/SampleHandler.h"
#include "Windows/CommandWindow.h"
#include "Windows/ItemSelectionWindow.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagScreenTest, "Unit Tests.Screens.BagScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagScreenTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UBagScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("REPEL"));
    auto &Subsystem = UPokemonSubsystem::GetInstance(World.Get());
    UReflectionUtils::SetPropertyValue<TScriptInterface<IBag>>(&Subsystem, "Bag", Bag);
    

    TWidgetPtr<UBagScreen> Screen(CreateWidget<UBagScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UItemSelectionWindow, ItemSelectionWindow);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UCommandWindow, CommandWindow);
    UE_ASSERT_NOT_NULL(CommandWindow);

    auto &HandlerSet = UReflectionUtils::GetMutablePropertyValue<TObjectPtr<UBagMenuHandlerSet>>(Screen.Get(), "CommandHandlers");
    HandlerSet = NewObject<UBagMenuHandlerSet>(Screen.Get());
    auto &Handlers = UReflectionUtils::GetMutablePropertyValue<TArray<TObjectPtr<UBagMenuHandler>>>(HandlerSet, "Handlers");
    Handlers.Empty();
    auto SampleHandler = NewObject<USampleHandler>(Screen.Get());
    Handlers.Emplace(SampleHandler);

    auto InputMappings = UReflectionUtils::GetPropertyValue<TObjectPtr<USelectionInputs>>(ItemSelectionWindow, "InputMappings");
    UE_ASSERT_NOT_NULL(InputMappings.Get());
    auto ConfirmButton = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "ConfirmInputs").begin();
    auto CancelButton = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "CancelInputs").begin();

    using enum ESlateVisibility;
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    UE_CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(ItemSelectionWindow->IsActive());
    UE_ASSERT_TRUE(CommandWindow->IsActive());
    UInputUtilities::SimulateKeyPress(CommandWindow, ConfirmButton);
    UE_CHECK_EQUAL(TEXT("REPEL"), SampleHandler->ItemID.ToString());
    UE_CHECK_EQUAL(1, SampleHandler->ItemQuantity);

    UInputUtilities::SimulateKeyPress(CommandWindow, CancelButton);
    UE_CHECK_EQUAL(Hidden, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(CommandWindow->IsActive());
    UE_ASSERT_TRUE(ItemSelectionWindow->IsActive());
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    UE_CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(ItemSelectionWindow->IsActive());
    UE_ASSERT_TRUE(CommandWindow->IsActive());
    CommandWindow->SetIndex(1);
    UInputUtilities::SimulateKeyPress(CommandWindow, ConfirmButton);
    UE_CHECK_EQUAL(Hidden, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(CommandWindow->IsActive());
    UE_ASSERT_TRUE(ItemSelectionWindow->IsActive());

    FName ItemID;
    int32 ItemQuantity;
    Screen->GetOnItemSelected().BindLambda(
        [&ItemID, &ItemQuantity](const TScriptInterface<IInventoryScreen> &, const FItem &Item, int32 Quantity) {
            ItemID = Item.ID;
            ItemQuantity = Quantity;
        });
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    UE_CHECK_EQUAL(TEXT("REPEL"), ItemID.ToString());
    UE_CHECK_EQUAL(1, ItemQuantity);

    return true;
}