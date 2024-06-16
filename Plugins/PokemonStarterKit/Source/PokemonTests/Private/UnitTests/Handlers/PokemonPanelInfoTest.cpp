
#include "Asserts.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "UtilityClasses/PokemonTestUtilities.h"
#include "Species/SpeciesData.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"
#include "Windows/PokemonSelectionPane.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonPanelInfoTest, "Private.UnitTests.UI.PokemonPanelInfoTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonPanelInfoTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonSelectionPane>();
    ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto ScreenClass = Subclasses[0];
    FPokemonTestUtilities::CreateMockParty(World.Get());

    auto Screen = CreateWidget<UPokemonSelectionPane>(World.Get(), ScreenClass);
    Screen->AddToViewport();

    auto ContentsArea = Screen->WidgetTree->FindWidget<UCanvasPanel>(TEXT("ContentsArea"));
    ASSERT_NOT_NULL(ContentsArea);

    TArray<UPokemonPanel *> Panels;
    for (int32 i = 0; i < 3; i++) {
        FName Name(FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}})));
        int32 OutIndex;
        auto Panel = Cast<UPokemonPanel>(Screen->WidgetTree->FindWidgetChild(ContentsArea, Name, OutIndex));
        ASSERT_NOT_NULL(ContentsArea);
        Panels.Add(Panel);
    }

    ASSERT_EQUAL(TEXT("SAMUROTT"), Panels[0]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());
    ASSERT_EQUAL(TEXT("EMBOAR"), Panels[1]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());
    ASSERT_EQUAL(TEXT("SERPERIOR"), Panels[2]->GetPokemon()->GetSpecies().ID.ToString().ToUpper());

    return true;
}