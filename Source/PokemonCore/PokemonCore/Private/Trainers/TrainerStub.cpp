// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/TrainerStub.h"

#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonBuilder.h"


FTrainerStub::FTrainerStub() {
	// Create the basic test party for now
	Party.Add(NewObject<UPokemonBuilder>()->Species("RIOLU")->Level(10)->Build());
	Party.Add(NewObject<UPokemonBuilder>()->Species("SNIVY")->Level(10)->Build());
	Party.Add(NewObject<UPokemonBuilder>()->Species("TEPIG")->Level(10)->Build());
	Party.Add(NewObject<UPokemonBuilder>()->Species("OSHAWOTT")->Level(10)->Build());
}

TArray<TSharedRef<IPokemon>>& FTrainerStub::GetParty() {
	return Party;
}

const TArray<TSharedRef<IPokemon>>& FTrainerStub::GetParty() const {
	return Party;
}
