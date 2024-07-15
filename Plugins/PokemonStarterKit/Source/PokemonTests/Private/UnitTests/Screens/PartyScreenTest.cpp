
#include "Asserts.h"
#include "Data/SelectionInputs.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PokemonSelectScreen.h"
#include "Species/SpeciesData.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/PokemonSelectionPane.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyScreenTest, "Unit Tests.Screens.PartyScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartyScreenTest::RunTest(const FString &Parameters) {
    using enum ESlateVisibility;

    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSelectScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];
    FPokemonTestUtilities::CreateMockParty(World.Get());

    TWidgetPtr<UPokemonSelectScreen> Screen(CreateWidget<UPokemonSelectScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UPokemonSelectionPane, SelectionPane);
    UE_ASSERT_NOT_NULL(SelectionPane);
    FIND_CHILD_WIDGET(Screen.Get(), UCommandWindow, CommandWindow);
    UE_ASSERT_NOT_NULL(CommandWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UHelpWindow, CommandHelpWindow);
    UE_ASSERT_NOT_NULL(CommandHelpWindow);

    UE_ASSERT_TRUE(SelectionPane->IsActivated());
    UE_CHECK_EQUAL(Collapsed, CommandWindow->GetVisibility());
    UE_CHECK_EQUAL(Collapsed, CommandHelpWindow->GetVisibility());
    
    auto InputMappings = UReflectionUtils::GetPropertyValue<TObjectPtr<USelectionInputs>>(SelectionPane, "InputMappings");
    UE_ASSERT_NOT_NULL(InputMappings.Get());
    
    auto ConfirmButton = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "ConfirmInputs").begin();
    auto Cancel = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "CancelInputs").begin();
    SelectionPane->GetSelectedOption()->OnClicked().Broadcast();
    UE_ASSERT_FALSE(SelectionPane->IsActivated());
    UE_ASSERT_TRUE(CommandWindow->IsActivated());
    UE_CHECK_EQUAL(Visible, CommandWindow->GetVisibility());
    UE_CHECK_EQUAL(SelfHitTestInvisible, CommandHelpWindow->GetVisibility());

    CommandWindow->SetIndex(CommandWindow->GetItemCount() - 1);
    CommandWindow->GetSelectedOption()->OnClicked().Broadcast();
    UE_ASSERT_TRUE(SelectionPane->IsActivated());
    UE_ASSERT_FALSE(CommandWindow->IsActivated());
    UE_CHECK_EQUAL(Collapsed, CommandWindow->GetVisibility());
    UE_CHECK_EQUAL(Collapsed, CommandHelpWindow->GetVisibility());

    SelectionPane->BeginSwitch(SelectionPane->GetIndex());
    SelectionPane->SetIndex(1);
    SelectionPane->GetSelectedOption()->OnClicked().Broadcast();

    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    UE_CHECK_EQUAL(TEXT("EMBOAR"), Trainer->GetParty()[0]->GetSpecies().ID.ToString());
    UE_CHECK_EQUAL(TEXT("SAMUROTT"), Trainer->GetParty()[1]->GetSpecies().ID.ToString());

    return true;
}