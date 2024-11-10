
#include "Components/Summary/SummaryPages.h"
#include "Asserts.h"
#include "CommonButtonBase.h"
#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Components/Summary/SummaryTabWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Input/UIActionBinding.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Screens/PokemonSummaryScreen.h"
#include "Species/SpeciesData.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SummaryPagesTest, "Unit Tests.Windows.SummaryPagesTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool SummaryPagesTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSummaryScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    TWidgetPtr<UPokemonSummaryScreen> Screen(CreateWidget<UPokemonSummaryScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FPokemonTestUtilities::CreateMockParty(World.Get());
    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    Screen->SetInitialPokemon(Trainer, 0);

    FIND_CHILD_WIDGET(Screen.Get(), USummaryPages, SummaryPages);
    UE_ASSERT_NOT_NULL(SummaryPages);
    FIND_CHILD_WIDGET(Screen.Get(), USummaryTabWidget, SummaryTabs);
    UE_ASSERT_NOT_NULL(SummaryTabs);
    FIND_CHILD_WIDGET(Screen.Get(), UPokemonSelectionPaneBase, PokemonSelection);
    UE_ASSERT_NOT_NULL(PokemonSelection);

    auto PreviousActionHandle = SummaryTabs->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuPrevious"; });
    UE_ASSERT_NOT_NULL(PreviousActionHandle);
    auto PreviousAction = FUIActionBinding::FindBinding(*PreviousActionHandle);
    UE_ASSERT_NOT_NULL(PreviousAction.Get());
    auto NextActionHandle = SummaryTabs->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuNext"; });
    UE_ASSERT_NOT_NULL(NextActionHandle);
    auto NextAction = FUIActionBinding::FindBinding(*NextActionHandle);
    UE_ASSERT_NOT_NULL(NextAction.Get());
    SummaryPages->SetPage(1);
    UE_CHECK_EQUAL(1, SummaryPages->GetCurrentPageIndex());

    UE_CHECK_TRUE(NextAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(2, SummaryPages->GetCurrentPageIndex());
    UE_CHECK_TRUE(PreviousAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(1, SummaryPages->GetCurrentPageIndex());

    UE_CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                   SummaryPages->GetPokemon()->GetSpecies().ID.ToString());
    PokemonSelection->SetIndex(1);
    PokemonSelection->GetSelectedOption()->SetIsSelected(true);
    UE_CHECK_EQUAL(Trainer->GetParty()[1]->GetSpecies().ID.ToString(),
                   SummaryPages->GetPokemon()->GetSpecies().ID.ToString());

    return true;
}