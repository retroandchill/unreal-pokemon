
#include "Asserts.h"
#include "Components/Summary/PokemonMovesPage.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ReflectionUtils.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonSummaryPagesTest_Moves,
                                 "Unit Tests.UI.Summary.Components.PokemonSummaryPagesTest.Moves",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonSummaryPagesTest_Moves::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();
    auto Subclasses = UReflectionUtils::GetAllSubclassesOfClass<UPokemonMovesPage>();
    UE_ASSERT_NOT_EQUAL(0, Subclasses.Num());
    auto WidgetClass = Subclasses[0];

    auto Page = CreateWidget<UPokemonMovesPage>(World.Get(), WidgetClass);
    Page->AddToViewport();

    auto ForeignTrainer =
        NewObject<UBasicTrainer>(World.Get())->Initialize(TEXT("LASS"), FText::FromStringView(TEXT("Amy")));
    auto Pokemon1 = UnrealInjector::NewInjectedDependency<IPokemon>(
        World.Get(), FPokemonDTO{.Species = "KABUTOPS", .Level = 40}, ForeignTrainer);
    Page->SetPokemon(Pokemon1);

    return true;
}