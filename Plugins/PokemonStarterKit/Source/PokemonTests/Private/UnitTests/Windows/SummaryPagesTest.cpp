
#include "Windows/SummaryPages.h"
#include "Asserts.h"
#include "Components/WidgetSwitcher.h"
#include "Data/SelectionInputs.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Species/SpeciesData.h"
#include "Utilities/InputUtilities.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryPagesTest, "Unit Tests.Windows.SummaryPagesTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryPagesTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryPages>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    TWidgetPtr<USummaryPages> Pages(CreateWidget<USummaryPages>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Pages.Get());
    Pages->AddToViewport();

    FPokemonTestUtilities::CreateMockParty(World.Get());
    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    Pages->SetInitialPokemon(Trainer->GetParty(), 0);
    Pages->ActivateWidget();

    FIND_CHILD_WIDGET(Pages.Get(), UWidgetSwitcher, PageSwitcher);
    UE_ASSERT_NOT_NULL(PageSwitcher);
    UE_CHECK_EQUAL(0, PageSwitcher->GetActiveWidgetIndex());

    auto InputMappings = UReflectionUtils::GetPropertyValue<TObjectPtr<USelectionInputs>>(Pages.Get(), "InputMappings");
    UE_ASSERT_NOT_NULL(InputMappings.Get());
    auto UpInput = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "UpInputs").begin();
    auto DownInput = *UReflectionUtils::GetPropertyValue<TSet<FKey>>(InputMappings, "DownInputs").begin();

    UE_CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UInputUtilities::SimulateKeyPress(Pages.Get(), DownInput);
    UE_CHECK_EQUAL(Trainer->GetParty()[1]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UInputUtilities::SimulateKeyPress(Pages.Get(), UpInput);
    UE_CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());

    Pages->SetPage(1);
    UE_CHECK_EQUAL(1, PageSwitcher->GetActiveWidgetIndex());

    return true;
}