// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/TrainerStub.h"

#include "Pokemon/GamePokemon.h"


FTrainerStub::FTrainerStub() {
	// Create the basic test party for now
	Party.Add(MakeShared<FGamePokemon>("PIKACHU", 10));
	Party.Add(MakeShared<FGamePokemon>("SNIVY", 10));
	Party.Add(MakeShared<FGamePokemon>("TEPIG", 10));
	Party.Add(MakeShared<FGamePokemon>("OSHAWOTT", 10));
	Party.Add(MakeShared<FGamePokemon>("AXEW", 10));
	Party.Add(MakeShared<FGamePokemon>("RIOLU", 10));
}

const TArray<TSharedRef<IPokemon>>& FTrainerStub::GetParty() {
	return Party;
}
