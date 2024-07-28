
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Screens/PokemonSelectScreen.h"
#include "Species/SpeciesData.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/RAII.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"
#include "Components/Party/PokemonSelectionPane.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PartyScreenTest, "Unit Tests.Screens.PartyScreenTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PartyScreenTest::RunTest(const FString &Parameters) {
    using enum ESlateVisibility;

    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSelectScreen>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];
    FPokemonTestUtilities::CreateMockParty(World.Get());

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    TWidgetPtr<UPokemonSelectScreen> Screen(CreateWidget<UPokemonSelectScreen>(World.Get(), WidgetClass));
    UE_ASSERT_NOT_NULL(Screen.Get());
    Screen->AddToViewport();

    FIND_CHILD_WIDGET(Screen.Get(), UPokemonSelectionPane, SelectionPane);
    UE_ASSERT_NOT_NULL(SelectionPane);

    SelectionPane->BeginSwitch(SelectionPane->GetIndex());
    SelectionPane->SetIndex(1);
    SelectionPane->GetSelectedOption()->OnClicked().Broadcast();

    auto Trainer = UPokemonSubsystem::GetInstance(World.Get()).GetPlayer();
    UE_CHECK_EQUAL(TEXT("EMBOAR"), Trainer->GetParty()[0]->GetSpecies().ID.ToString());
    UE_CHECK_EQUAL(TEXT("SAMUROTT"), Trainer->GetParty()[1]->GetSpecies().ID.ToString());

    return true;
}