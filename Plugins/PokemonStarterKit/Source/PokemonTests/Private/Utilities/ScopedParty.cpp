// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/ScopedParty.h"

namespace Pokemon::Tests {

    FScopedParty::~FScopedParty() {
        if (!Trainer.IsValid()) {
            return;
        }

        Trainer->ClearParty();
    }

    const TScriptInterface<IPokemon> & FScopedParty::GetPokemon(int32 Index) const {
        check(Pokemon.IsValidIndex(Index))
        return Pokemon[Index];
    }
}