// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Lookup/InjectionUtilities.h"
#include "Pokemon/Pokemon.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Unit Tests.Core.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString &Parameters) {
    auto [DudOverlay, World] = UWidgetTestUtilities::CreateTestWorld();

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UnrealInjector::NewInjectedDependency<IPokemon>(World, FPokemonDTO{.Species = "LUCARIO", .Level = 64}));
    Trainer->AddPokemonToParty(UnrealInjector::NewInjectedDependency<IPokemon>(World, FPokemonDTO{.Species = "MIMIKYU", .Level = 44}));

    ASSERT_EQUAL(2640, Trainer->GetPayout());

    return true;
}
#endif