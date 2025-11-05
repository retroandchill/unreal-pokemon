// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PartyManagementHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Storage/StorageSystem.h"

TScriptInterface<IPokemon> UPartyManagementHelpers::AddPokemonToParty(const UObject *WorldContext,
                                                                      const FPokemonDTO &Pokemon) {
    auto &Subsystem = UPokemonSubsystem::GetInstance(WorldContext);
    auto Player = Subsystem.GetPlayer();
    check(Player != nullptr)
    auto CreatedPokemon = UnrealInjector::NewInjectedDependency<IPokemon>(Player.GetObject(), Pokemon);
    if (Player->IsPartyFull()) {
        auto StorageSystem = Subsystem.GetStorageSystem();
        check(StorageSystem != nullptr)
        StorageSystem->TryDeposit(CreatedPokemon);
    } else {
        Player->AddPokemonToParty(CreatedPokemon);
    }
    return CreatedPokemon;
}