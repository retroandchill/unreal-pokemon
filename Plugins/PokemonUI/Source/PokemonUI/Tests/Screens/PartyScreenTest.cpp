#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "../Tests/PokemonTestUtilities.h"
#include "Asserts.h"
#include "Data/SelectionInputs.h"
#include "External/accessor.hpp"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PokemonSelectScreen.h"
#include "Species/SpeciesData.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/PokemonSelectionPane.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessInputMappingsPartyScreen, USelectableWidget, InputMappings, TObjectPtr<USelectionInputs>)
MEMBER_ACCESSOR(AccessConfirmInput, USelectionInputs, ConfirmInputs, TSet<FKey>)
MEMBER_ACCESSOR(AccessCancelInput, USelectionInputs, CancelInputs, TSet<FKey>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyScreenTest, "Unit Tests.Screens.PartyScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartyScreenTest::RunTest(const FString &Parameters) {
    using enum ESlateVisibility;

    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSelectScreen>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];
    FPokemonTestUtilities::CreateMockParty(World.Get());

    TWidgetPtr<UPokemonSelectScreen> Screen(CreateWidget<UPokemonSelectScreen>(World.Get(), WidgetClass));
    ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UPokemonSelectionPane, SelectionPane);
    ASSERT_NOT_NULL(SelectionPane);
    FIND_CHILD_WIDGET(Screen.Get(), UCommandWindow, CommandWindow);
    ASSERT_NOT_NULL(CommandWindow);
    FIND_CHILD_WIDGET(Screen.Get(), UHelpWindow, CommandHelpWindow);
    ASSERT_NOT_NULL(CommandHelpWindow);

    ASSERT_TRUE(SelectionPane->IsActive());
    CHECK_EQUAL(Collapsed, CommandWindow->GetVisibility());
    CHECK_EQUAL(Collapsed, CommandHelpWindow->GetVisibility());

    USelectionInputs *InputMappings = accessMember<AccessInputMappingsPartyScreen>(*SelectionPane).get();
    ASSERT_NOT_NULL(InputMappings);
    auto ConfirmButton = *accessMember<AccessConfirmInput>(*InputMappings).get().CreateIterator();
    auto Cancel = *accessMember<AccessCancelInput>(*InputMappings).get().CreateIterator();
    UInputUtilities::SimulateKeyPress(SelectionPane, ConfirmButton);
    ASSERT_FALSE(SelectionPane->IsActive());
    ASSERT_TRUE(CommandWindow->IsActive());
    CHECK_EQUAL(Visible, CommandWindow->GetVisibility());
    CHECK_EQUAL(SelfHitTestInvisible, CommandHelpWindow->GetVisibility());

    CommandWindow->SetIndex(CommandWindow->GetItemCount() - 1);
    UInputUtilities::SimulateKeyPress(CommandWindow, ConfirmButton);
    ASSERT_TRUE(SelectionPane->IsActive());
    ASSERT_FALSE(CommandWindow->IsActive());
    CHECK_EQUAL(Collapsed, CommandWindow->GetVisibility());
    CHECK_EQUAL(Collapsed, CommandHelpWindow->GetVisibility());

    SelectionPane->BeginSwitch(SelectionPane->GetIndex());
    SelectionPane->SetIndex(1);
    UInputUtilities::SimulateKeyPress(SelectionPane, ConfirmButton);

    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    CHECK_EQUAL(TEXT("EMBOAR"), Trainer->GetParty()[0]->GetSpecies().ID.ToString());
    CHECK_EQUAL(TEXT("SAMUROTT"), Trainer->GetParty()[1]->GetSpecies().ID.ToString());

    return true;
}
#endif