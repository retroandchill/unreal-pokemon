// "Unreal Pokémon" created by Retro & Chill.
#include "Managers/PokemonSubsystem.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonDTO.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Species/SpeciesData.h"

#include "Utilities/fakeit.hpp"
#include "Utilities/PokemonTestUtilities.h"

using namespace fakeit;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DefaultStatBlockTest, "UnrealPokemon.Core.Stats.DefaultStatBlockTest",
                                 EAutomationTestFlags::ApplicationContextMask  | EAutomationTestFlags::ProductFilter)

bool DefaultStatBlockTest::RunTest(const FString& Parameters) {
	FGameInstancePtr GameInstance;
	if (!UPokemonSubsystem::Exists()) {
		GameInstance.Reset(NewObject<UGameInstance>());
		GameInstance->Init();
	}

	FPokemonDTO PokemonDTO = { .Species = TEXT("GARCHOMP"), .Level = 78, .Nature = "ADAMANT" };
	PokemonDTO.IVs = {
		{"HP", 24},
		{"ATTACK", 12},
		{"DEFENSE", 30},
		{"SPECIAL_ATTACK", 16},
		{"SPECIAL_DEFENSE", 23},
		{"SPEED", 5}
	};

	PokemonDTO.EVs = {
		{"HP", 74},
		{"ATTACK", 190},
		{"DEFENSE", 91},
		{"SPECIAL_ATTACK", 48},
		{"SPECIAL_DEFENSE", 84},
		{"SPEED", 23}
	};
	
	auto NewPokemon = NewObject<UGamePokemon>();
	NewPokemon->Initialize(PokemonDTO);

	auto Block = NewObject<UDefaultStatBlock>();
	Block->Initialize(NewPokemon, PokemonDTO);
	auto &Species = NewPokemon->GetSpecies();
	Block->CalculateStats(Species.BaseStats);

	int32 HP = Block->GetStat("HP")->GetStatValue();
	bool Passed = TestEqual("HP", HP, 289);

	int32 Atk = Block->GetStat("ATTACK")->GetStatValue();
	Passed &= TestEqual("Attack", Atk, 278);

	int32 Def = Block->GetStat("DEFENSE")->GetStatValue();
	Passed &= TestEqual("Defense", Def, 193);

	int32 SpA = Block->GetStat("SPECIAL_ATTACK")->GetStatValue();
	Passed &= TestEqual("Sp. Atk", SpA, 135);

	int32 SpD = Block->GetStat("SPECIAL_DEFENSE")->GetStatValue();
	Passed &= TestEqual("Sp. Def", SpD, 171);

	int32 Spe = Block->GetStat("SPEED")->GetStatValue();
	Passed &= TestEqual("Speed", Spe, 171);

	int32 Exp = Block->GetExp();
	Passed &= TestEqual("Exp.", Exp, 593190);

	int32 NextLevel = Block->GetExpForNextLevel();
	Passed &= TestEqual("Next Level Exp.", NextLevel, 616298);
	
	return Passed;
}
