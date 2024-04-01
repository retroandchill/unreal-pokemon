// "Unreal Pokémon" created by Retro & Chill.
#include "CommonDefines.h"
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Utilities/accessor.hpp"
#include "Trainers/BasicTrainer.h"
#include "Utilities/fakeit.hpp"

using namespace accessor;
using namespace fakeit;

MEMBER_ACCESSOR(AccessParty, FBasicTrainer, Party, TArray<TSharedRef<IPokemon>>)

/**
 * Mock trainer type made to expose the constructor
 */
class FMockTrainer : public FBasicTrainer {
public:
	FMockTrainer(FName TrainerType, const FText &Name) : FBasicTrainer(TrainerType, Name) {
		
	}
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(TestTrainerPayout, "Private.PokemonCore.Trainers.TestTrainerPayout",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool TestTrainerPayout::RunTest(const FString& Parameters) {
	// Make the test pass by returning true, or fail by returning false.
	
	Mock<IPokemon> MockPokemon1;
	Fake(Dtor(MockPokemon1));

	Mock<IStatBlock> MockStatBlock;
	When(Method(MockStatBlock, GetLevel)).Return(44);
	Mock<IPokemon> MockPokemon2;
	Fake(Dtor(MockPokemon2));
	When(Method(MockPokemon2, GetStatBlock)).Return(MockStatBlock.get());

	TArray<TSharedRef<IPokemon>> Party;
	Party.Add(TSharedRef<IPokemon>(&MockPokemon1.get()));
	Party.Add(TSharedRef<IPokemon>(&MockPokemon2.get()));
	
	FMockTrainer Trainer(TEXT("POKEMONRANGER_M"), FText::FromStringView(TEXT("Test")));
	accessMember<AccessParty>(Trainer).get() = Party;
	AUTOMATION_ASSERT(TestEqual(TEXT(""), Trainer.GetPayout(), 2640));
	
	return true;
}
