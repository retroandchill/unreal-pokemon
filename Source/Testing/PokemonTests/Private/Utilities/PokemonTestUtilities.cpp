// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PokemonTestUtilities.h"

#include "Asserts.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/PokemonBuilder.h"

void UPokemonTestUtilities::CreateMockParty(UObject* WorldContext) {
	using enum EPokemonGender;

	auto PokemonSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>();
	ASSERT(PokemonSubsystem != nullptr);

	auto& Party = PokemonSubsystem->GetPlayer().GetParty();
	Party.Empty();

	Party.Add(NewObject<UPokemonBuilder>()
	          ->Species(TEXT("SAMUROTT"))
	          ->Level(100)
	          ->Gender(Male)
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
	          ->Build());
	Party.Add(NewObject<UPokemonBuilder>()
			  ->Species(TEXT("EMBOAR"))
			  ->Level(100)
			  ->Gender(Female)
			  ->IV("HP", 31)
			  ->IV("ATTACK", 31)
			  ->IV("DEFENSE", 31)
			  ->IV("SPECIAL_ATTACK", 31)
			  ->IV("SPECIAL_DEFENSE", 31)
			  ->IV("SPEED", 31)
			  ->EV("ATTACK", 252)
			  ->EV("DEFENSE", 4)
			  ->EV("SPEED", 252)
			  ->Nature("Jolly")
			  ->Ability(2)
			  ->Moves({"FLAREBLITZ", "SUPERPOWER", "WILDCHARGE", "EARTHQUAKE"})
			  ->Item("CHOICEBAND")
			  ->Shiny(true)
			  ->Build());
	Party.Add(NewObject<UPokemonBuilder>()
			  ->Species(TEXT("SERPERIOR"))
			  ->Level(100)
			  ->Gender(Female)
			  ->IV("HP", 31)
			  ->IV("ATTACK", 0)
			  ->IV("DEFENSE", 31)
			  ->IV("SPECIAL_ATTACK", 31)
			  ->IV("SPECIAL_DEFENSE", 31)
			  ->IV("SPEED", 31)
			  ->EV("HP", 208)
			  ->EV("DEFENSE", 48)
			  ->EV("SPEED", 252)
			  ->Nature("Timid")
			  ->Ability(2)
			  ->Moves({"LEAFSTORM", "TAUNT", "GLARE", "DEFOG"})
			  ->Item("ROCKYHELMET")
			  ->Shiny(false)
			  ->Build());
}
