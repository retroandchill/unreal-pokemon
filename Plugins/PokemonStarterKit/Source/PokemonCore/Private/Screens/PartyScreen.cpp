#include "Screens/PartyScreen.h"
#include "Trainers/Trainer.h"

FSelectedPokemonHandle::FSelectedPokemonHandle(const TScriptInterface<IPartyScreen> &InScreen,
                                               const TScriptInterface<ITrainer> &InTrainer, int32 InIndex)
    : Screen(InScreen), Trainer(InTrainer), Index(InIndex) {
}

TScriptInterface<IPokemon> FSelectedPokemonHandle::GetPokemon() const {
    return Trainer->GetPokemon(Index);
}
