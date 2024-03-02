// "Unreal Pokémon" created by Retro & Chill.
#include "Managers/PokemonManager.h"

#include "Trainers/TrainerStub.h"

// TODO: Swap this instantiation with the actual trainer instantiation
FPokemonManager::FPokemonManager() : Player(MakeUnique<FTrainerStub>()) {
}

FPokemonManager::~FPokemonManager() = default;

FPokemonManager& FPokemonManager::GetInstance() {
	static FPokemonManager Instance;
	return Instance;
}

ITrainer& FPokemonManager::GetPlayer() {
	return *Player;
}

const ITrainer& FPokemonManager::GetPlayer() const {
	return *Player;
}
