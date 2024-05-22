// "Unreal Pokémon" created by Retro & Chill.
#if WITH_TESTS && HAS_AUTOMATION_HELPERS
#include "Asserts.h"
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/PokemonDTO.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ConstructionUtilities.h"
#include "Utilities/RAII.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Unit Tests.Core.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString &Parameters) {
    FGameInstancePtr GameInstance;
    if (!UPokemonSubsystem::Exists()) {
        GameInstance.Reset(NewObject<UGameInstance>());
        GameInstance->Init();
    }

    auto Trainer = NewObject<UBasicTrainer>()->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
    Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "LUCARIO", .Level = 64}));
    Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "MIMIKYU", .Level = 44}));

    ASSERT_EQUAL(2640, Trainer->GetPayout());

    return true;
}
#endif