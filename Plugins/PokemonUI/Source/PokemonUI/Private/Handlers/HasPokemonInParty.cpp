// "Unreal Pokémon" created by Retro & Chill.


#include "Handlers/HasPokemonInParty.h"
#include "Managers/PokemonSubsystem.h"

bool UHasPokemonInParty::IsEnabled_Implementation() const {
#ifdef WITH_EDITOR
    // If the system exists then we're probably in edit mode
    if (!UPokemonSubsystem::Exists()) {
        return true;
    }
#endif
    
    auto Player = UPokemonSubsystem::GetInstance().GetPlayer();
    check(Player != nullptr)
    return !Player->GetParty().IsEmpty();
}