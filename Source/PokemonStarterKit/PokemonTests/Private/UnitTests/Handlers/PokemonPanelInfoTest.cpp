
#include "Asserts.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/Party/PokemonSelectionPane.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Species/SpeciesData.h"
#include "Utilities/PlayerUtilities.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "UtilityClasses/PokemonTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonPanelInfoTest, "Unit Tests.UI.PokemonPanelInfoTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonPanelInfoTest::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSelectionPane>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto ScreenClass = Subclasses[0];
    FPokemonTestUtilities::CreateMockParty(World.Get());

    auto [Player, Pawn] = UPlayerUtilities::CreateTestPlayer(*World);
    auto Screen = CreateWidget<UPokemonSelectionPane>(World.Get(), ScreenClass);
    Screen->AddToViewport();

    TArray<UPokemonPanel *> Panels;
    for (int32 i = 0; i < 3; i++)
    {
        FName Name(FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}})));
        auto Panel = Screen->WidgetTree->FindWidget<UPokemonPanel>(Name);
        UE_ASSERT_NOT_NULL(Panel);
        Panels.Add(Panel);
    }

    UE_ASSERT_EQUAL(TEXT("SAMUROTT"), Panels[0]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());
    UE_ASSERT_EQUAL(TEXT("EMBOAR"), Panels[1]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());
    UE_ASSERT_EQUAL(TEXT("SERPERIOR"), Panels[2]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());

    return true;
}