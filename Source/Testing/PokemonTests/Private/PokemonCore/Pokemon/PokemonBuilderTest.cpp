// "Unreal Pokémon" created by Retro & Chill.


#include "PokemonCore/Pokemon/PokemonBuilderTest.h"

#include "CommonDefines.h"
#include "Memory/GCPointer.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/PokemonBuilder.h"


// Sets default values
APokemonBuilderTest::APokemonBuilderTest() {
	OnTestStart.AddDynamic(this, &APokemonBuilderTest::RunTest);
}

void APokemonBuilderTest::RunTest() {
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

	auto Builder = TGCPointer(Pokemon1->ToBuilder());

	auto Pokemon2 = Builder->Build();
	
	TEST_ASSERT(AssertTrue(*Pokemon1 == *Pokemon2, TEXT("Both Pokémon should be the same!")))
}


