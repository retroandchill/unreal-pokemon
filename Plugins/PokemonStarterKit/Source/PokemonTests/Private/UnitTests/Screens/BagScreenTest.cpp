#include "Screens/BagScreen.h"
#include "Asserts.h"
#include "Data/SelectionInputs.h"
#include "DependencyInjectionSubsystem.h"
#include "External/accessor.hpp"
#include "Handlers/BagMenu/BagMenuHandlerSet.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "RPGMenusSubsystem.h"
#include "SampleHandler.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/ItemSelectionWindow.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessInputMappingsBag, USelectableWidget, InputMappings, TObjectPtr<USelectionInputs>)
MEMBER_ACCESSOR(AccessBag, UPokemonSubsystem, Bag, TScriptInterface<IBag>)
MEMBER_ACCESSOR(AccessHandler, UBagScreen, CommandHandlers, TObjectPtr<UBagMenuHandlerSet>)
MEMBER_ACCESSOR(AccessBagHandlers, UBagMenuHandlerSet, Handlers, TArray<TObjectPtr<UBagMenuHandler>>)
MEMBER_ACCESSOR(AccessConfirmInputBag, USelectionInputs, ConfirmInputs, TSet<FKey>)
MEMBER_ACCESSOR(AccessCancelInputBag, USelectionInputs, CancelInputs, TSet<FKey>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(BagScreenTest, "Private.UnitTests.Screens.BagScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool BagScreenTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UBagScreen>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Bag = UnrealInjector::NewInjectedDependency<IBag>(World.Get());
    Bag->ObtainItem(TEXT("REPEL"));
    auto &Subsystem = UPokemonSubsystem::GetInstance(World.Get());
    accessMember<AccessBag>(Subsystem).get() = Bag;

    TWidgetPtr<UBagScreen> Screen(CreateWidget<UBagScreen>(World.Get(), WidgetClass));
    ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UItemSelectionWindow, ItemSelectionWindow);
    ASSERT_NOT_NULL(ItemSelectionWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UCommandWindow, CommandWindow);
    ASSERT_NOT_NULL(CommandWindow);

    auto &HandlerSet = accessMember<AccessHandler>(*Screen).get();
    HandlerSet = NewObject<UBagMenuHandlerSet>(Screen.Get());
    auto &Handlers = accessMember<AccessBagHandlers>(*HandlerSet).get();
    Handlers.Empty();
    auto SampleHandler = NewObject<USampleHandler>(Screen.Get());
    Handlers.Emplace(SampleHandler);

    USelectionInputs *InputMappings = accessMember<AccessInputMappingsBag>(*ItemSelectionWindow).get();
    ASSERT_NOT_NULL(InputMappings);
    auto ConfirmButton = *accessMember<AccessConfirmInputBag>(*InputMappings).get().CreateIterator();
    auto CancelButton = *accessMember<AccessCancelInputBag>(*InputMappings).get().CreateIterator();

    using enum ESlateVisibility;
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    CHECK_FALSE(ItemSelectionWindow->IsActive());
    ASSERT_TRUE(CommandWindow->IsActive());
    UInputUtilities::SimulateKeyPress(CommandWindow, ConfirmButton);
    CHECK_EQUAL(TEXT("REPEL"), SampleHandler->ItemID.ToString());
    CHECK_EQUAL(1, SampleHandler->ItemQuantity);

    UInputUtilities::SimulateKeyPress(CommandWindow, CancelButton);
    CHECK_EQUAL(Hidden, CommandWindow->GetVisibility());
    CHECK_FALSE(CommandWindow->IsActive());
    ASSERT_TRUE(ItemSelectionWindow->IsActive());
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    CHECK_FALSE(ItemSelectionWindow->IsActive());
    ASSERT_TRUE(CommandWindow->IsActive());
    CommandWindow->SetIndex(1);
    UInputUtilities::SimulateKeyPress(CommandWindow, ConfirmButton);
    CHECK_EQUAL(Hidden, CommandWindow->GetVisibility());
    CHECK_FALSE(CommandWindow->IsActive());
    ASSERT_TRUE(ItemSelectionWindow->IsActive());

    FName ItemID;
    int32 ItemQuantity;
    Screen->GetOnItemSelected().BindLambda(
        [&ItemID, &ItemQuantity](const TScriptInterface<IInventoryScreen> &, const FItem &Item, int32 Quantity) {
            ItemID = Item.ID;
            ItemQuantity = Quantity;
        });
    UInputUtilities::SimulateKeyPress(ItemSelectionWindow, ConfirmButton);
    CHECK_EQUAL(TEXT("REPEL"), ItemID.ToString());
    CHECK_EQUAL(1, ItemQuantity);

    return true;
}