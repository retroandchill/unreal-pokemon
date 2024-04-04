// "Unreal Pokémon" created by Retro & Chill.


#include "Trainers/TrainerStub.h"

#include "Pokemon/GamePokemon.h"
#include "Utilities/ConstructionUtilities.h"

TScriptInterface<ITrainer> UTrainerStub::Initialize(FName NewTrainerType, FText NewTrainerName) {
	auto Ret = Super::Initialize(NewTrainerType, NewTrainerName);
	
	AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "RIOLU", .Level = 10}));
	AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "SNIVY", .Level = 10}));
	AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "TEPIG", .Level = 10}));
	AddPokemonToParty(UConstructionUtilities::CreateNewPokemon({.Species = "OSHAWOTT", .Level = 10}));

	return this;
}

TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() {
	return Party;
}

const TArray<TScriptInterface<IPokemon>>& UTrainerStub::GetParty() const {
	return Party;
}