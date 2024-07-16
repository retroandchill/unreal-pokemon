
#include "Windows/SummaryPages.h"
#include "Asserts.h"
#include "Components/WidgetSwitcher.h"
#include "Input/UIActionBinding.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
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
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<USummaryPages>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
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

    auto PreviousActionHandle =
        Pages->GetActionBindings().FindByPredicate([](const FUIActionBindingHandle &BindingHandle) {
            return BindingHandle.GetActionName() == "MenuPreviousPokemon";
        });
    UE_ASSERT_NOT_NULL(PreviousActionHandle);
    auto PreviousAction = FUIActionBinding::FindBinding(*PreviousActionHandle);
    UE_ASSERT_NOT_NULL(PreviousAction.Get());
    auto NextActionHandle = Pages->GetActionBindings().FindByPredicate(
        [](const FUIActionBindingHandle &BindingHandle) { return BindingHandle.GetActionName() == "MenuNextPokemon"; });
    UE_ASSERT_NOT_NULL(NextActionHandle);
    auto NextAction = FUIActionBinding::FindBinding(*NextActionHandle);
    UE_ASSERT_NOT_NULL(NextAction.Get());

    UE_CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                   Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UE_CHECK_TRUE(NextAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(Trainer->GetParty()[1]->GetSpecies().ID.ToString(),
                   Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());
    UE_CHECK_TRUE(PreviousAction->OnExecuteAction.ExecuteIfBound());
    UE_CHECK_EQUAL(Trainer->GetParty()[0]->GetSpecies().ID.ToString(),
                   Pages->GetCurrentPokemon()->GetSpecies().ID.ToString());

    Pages->SetPage(1);
    UE_CHECK_EQUAL(1, PageSwitcher->GetActiveWidgetIndex());

    return true;
}