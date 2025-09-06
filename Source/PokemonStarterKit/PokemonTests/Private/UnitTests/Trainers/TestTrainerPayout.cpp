// "Unreal Pokémon" created by Retro & Chill.

#include "Asserts.h"
#include "Lookup/InjectionUtilities.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/WidgetTestUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Unit Tests.Core.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString &Parameters)
{
    auto [DudOverlay, World, GameInstance] = UWidgetTestUtilities::CreateTestWorld();

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "LUCARIO", .Level = 64}));
    Trainer->AddPokemonToParty(
        UnrealInjector::NewInjectedDependency<IPokemon>(World.Get(), FPokemonDTO{.Species = "MIMIKYU", .Level = 44}));

    UE_ASSERT_EQUAL(2640, Trainer->GetPayout());

    return true;
}