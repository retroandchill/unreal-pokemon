// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/TrainerStub.h"

#include "Pokemon/GamePokemon.h"
#include "Pokemon/PokemonBuilder.h"

void UTrainerStub::Initialize() {
	// Create the basic test party for now
	Party.Add(NewObject<UPokemonBuilder>()->Species("RIOLU")->Level(10)->Build(this));
	Party.Add(NewObject<UPokemonBuilder>()->Species("SNIVY")->Level(10)->Build(this));
	Party.Add(NewObject<UPokemonBuilder>()->Species("TEPIG")->Level(10)->Build(this));
	Party.Add(NewObject<UPokemonBuilder>()->Species("OSHAWOTT")->Level(10)->Build(this));
}

TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() {
	return Party;
}

const TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() const {
	return Party;
}
