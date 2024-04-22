// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Assertions/Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ConstructionUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Private.PokemonCore.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString &Parameters) {
    // Make the test pass by returning true, or fail by returning false.

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "LUCARIO", .Level = 64}));
    Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "MIMIKYU", .Level = 44}));

    ASSERT_EQUAL(2640, Trainer->GetPayout());

    return true;
}
#endif