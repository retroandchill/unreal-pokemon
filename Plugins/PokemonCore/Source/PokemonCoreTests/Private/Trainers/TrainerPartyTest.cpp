#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Species/SpeciesData.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TrainerPartyTest, "Private.Trainers.TrainerPartyTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TrainerPartyTest::RunTest(const FString &Parameters) {
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto Trainer = NewObject<UBasicTrainer>(World.Get())->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "LUCARIO", .Level = 64}));
    Trainer->AddPokemonToParty(
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "MIMIKYU", .Level = 44}));

    ASSERT_EQUAL(2, Trainer->GetParty().Num());
    CHECK_EQUAL(TEXT("LUCARIO"), Trainer->GetPokemon(0)->GetSpecies().ID.ToString());
    CHECK_EQUAL(TEXT("MIMIKYU"), Trainer->GetPokemon(1)->GetSpecies().ID.ToString());
    
    return true;
}