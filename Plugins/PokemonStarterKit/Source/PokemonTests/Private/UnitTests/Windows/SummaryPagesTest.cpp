
#include "Windows/SummaryPages.h"
#include "Asserts.h"
#include "Components/WidgetSwitcher.h"
#include "Data/SelectionInputs.h"
#include "External/accessor.hpp"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Species/SpeciesData.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"

using namespace accessor;

MEMBER_ACCESSOR(AccessInputMappingsPages, USelectableWidget, InputMappings, TObjectPtr<USelectionInputs>)
MEMBER_ACCESSOR(AccessUpInput, USelectionInputs, UpInputs, TSet<FKey>)
MEMBER_ACCESSOR(AccessDownInput, USelectionInputs, DownInputs, TSet<FKey>)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryPagesTest, "Private.UnitTests.Windows.SummaryPagesTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryPagesTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryPages>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<USummaryPages> Pages(CreateWidget<USummaryPages>(World.Get(), WidgetClass));
    ASSERT_NOT_NULL(Pages.Get());
    Pages->AddToViewport();

    FPokemonTestUtilities::CreateMockParty(World.Get());
    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    Pages->SetInitialPokemon(Trainer->GetParty(), 0);
    Pages->SetActive(true);

    FIND_CHILD_WIDGET(Pages.Get(), UWidgetSwitcher, PageSwitcher);
    ASSERT_NOT_NULL(PageSwitcher);
    CHECK_EQUAL(0, PageSwitcher->GetActiveWidgetIndex());

    USelectionInputs *InputMappings = accessMember<AccessInputMappingsPages>(*Pages).get();
    ASSERT_NOT_NULL(InputMappings);
    auto UpInput = *accessMember<AccessUpInput>(*InputMappings).get().CreateIterator();
    auto DownInput = *accessMember<AccessDownInput>(*InputMappings).get().CreateIterator();

    CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UInputUtilities::SimulateKeyPress(Pages.Get(), DownInput);
    CHECK_EQUAL(Trainer->GetParty()[1]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UInputUtilities::SimulateKeyPress(Pages.Get(), UpInput);
    CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());

    Pages->SetIndex(1);
    CHECK_EQUAL(1, PageSwitcher->GetActiveWidgetIndex());

    return true;
}