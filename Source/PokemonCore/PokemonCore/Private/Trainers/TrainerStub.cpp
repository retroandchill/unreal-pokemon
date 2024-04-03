// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/TrainerStub.h"

#include "Pokemon/GamePokemon.h"
#include "Utilities/ConstructionUtilities.h"

UTrainerStub* UTrainerStub::Initialize() {
	// Create the basic test party for now
	Party.Add(UConstructionUtilities::CreateNewPokemon({.Species = "RIOLU", .Level = 10}));
	Party.Add(UConstructionUtilities::CreateNewPokemon({.Species = "SNIVY", .Level = 10}));
	Party.Add(UConstructionUtilities::CreateNewPokemon({.Species = "TEPIG", .Level = 10}));
	Party.Add(UConstructionUtilities::CreateNewPokemon({.Species = "OSHAWOTT", .Level = 10}));
	return this;
}

TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() {
	return Party;
}

const TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() const {
	return Party;
}
