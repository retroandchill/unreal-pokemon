#include "Screens/BagScreen.h"
#include "Asserts.h"
#include "CommonButtonBase.h"
#include "Handlers/BagMenu/BagMenuHandlerSet.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Player/Bag.h"
#include "Utilities/PlayerUtilities.h"
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

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    TWidgetPtr<UBagScreen> Screen(CreateWidget<UBagScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UItemSelectionWindow, ItemSelectionWindow);
    UE_ASSERT_NOT_NULL(ItemSelectionWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UCommandWindow, CommandWindow);
    UE_ASSERT_NOT_NULL(CommandWindow);

    auto &HandlerSet =
        UReflectionUtils::GetMutablePropertyValue<TObjectPtr<UBagMenuHandlerSet>>(Screen.Get(), "CommandHandlers");
    HandlerSet = NewObject<UBagMenuHandlerSet>(Screen.Get());
    auto &Handlers =
        UReflectionUtils::GetMutablePropertyValue<TArray<TObjectPtr<UBagMenuHandler>>>(HandlerSet, "Handlers");
    Handlers.Empty();
    auto SampleHandler = NewObject<USampleHandler>(Screen.Get());
    Handlers.Emplace(SampleHandler);

    using enum ESlateVisibility;
    ItemSelectionWindow->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(ItemSelectionWindow->IsActivated());
    UE_ASSERT_TRUE(CommandWindow->IsActivated());
    CommandWindow->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(TEXT("REPEL"), SampleHandler->ItemID.ToString());
    UE_CHECK_EQUAL(1, SampleHandler->ItemQuantity);

    CommandWindow->GetOnCancel().Broadcast();
    UE_CHECK_EQUAL(Hidden, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(CommandWindow->IsActivated());
    UE_ASSERT_TRUE(ItemSelectionWindow->IsActivated());
    ItemSelectionWindow->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(SelfHitTestInvisible, CommandWindow->GetVisibility());
    UE_CHECK_FALSE(ItemSelectionWindow->IsActivated());
    UE_ASSERT_TRUE(CommandWindow->IsActivated());
    CommandWindow->GetOnCancel().Broadcast();

    FName ItemID;
    int32 ItemQuantity;
    Screen->GetOnItemSelected().BindLambda(
        [&ItemID, &ItemQuantity](const TScriptInterface<IInventoryScreen> &, const FItem &Item, int32 Quantity) {
            ItemID = Item.ID;
            ItemQuantity = Quantity;
        });
    ItemSelectionWindow->GetSelectedOption()->OnClicked().Broadcast();
    UE_CHECK_EQUAL(TEXT("REPEL"), ItemID.ToString());
    UE_CHECK_EQUAL(1, ItemQuantity);

    return true;
}