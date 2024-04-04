// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Misc/AutomationTest.h"
#include "Trainers/BasicTrainer.h"
#include "Utilities/ConstructionUtilities.h"
#include "Pokemon/PokemonDTO.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Private.PokemonCore.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	
	auto Trainer = NewObject<UBasicTrainer>()
		->Initialize(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
	Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "LUCARIO", .Level = 64}));
	Trainer->AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "MIMIKYU", .Level = 44}));

	AUTOMATION_ASSERT(TestEqual(TEXT(""), Trainer->GetPayout(), 2640));
	
	return true;
}
