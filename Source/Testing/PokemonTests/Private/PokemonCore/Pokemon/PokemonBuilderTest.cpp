// "Unreal Pokémon" created by Retro & Chill.
#include "Misc/AutomationTest.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonBuilder.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(PokemonBuilderTest, "Private.PokemonCore.Pokemon.PokemonBuilderTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool PokemonBuilderTest::RunTest(const FString& Parameters) {
	auto Pokemon1 = NewObject<UPokemonBuilder>()
			  ->Species(TEXT("SAMUROTT"))
			  ->Level(100)
			  ->Gender(EPokemonGender::Male)
			  ->IV("HP", 31)
			  ->IV("ATTACK", 31)
			  ->IV("DEFENSE", 31)
			  ->IV("SPECIAL_ATTACK", 31)
			  ->IV("SPECIAL_DEFENSE", 31)
			  ->IV("SPEED", 31)
			  ->EV("ATTACK", 164)
			  ->EV("SPECIAL_ATTACK", 92)
			  ->EV("SPEED", 252)
			  ->Nature("Naive")
			  ->Ability(0)
			  ->Moves({"HYDROPUMP", "KNOCKOFF", "MEGAHORN", "SACREDSWORD"})
			  ->Item("LIFEORB")
			  ->Shiny(false)
			  ->Build();

	auto Builder = Pokemon1->ToBuilder();

	auto Pokemon2 = Builder->Build();
	
	return TestEqual(TEXT("Both Pokémon should be the same!"), *Pokemon1, *Pokemon2);
}
