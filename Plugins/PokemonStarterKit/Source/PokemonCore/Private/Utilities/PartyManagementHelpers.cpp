// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/PartyManagementHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Lookup/InjectionUtilities.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"

TScriptInterface<IPokemon> UPartyManagementHelpers::AddPokemonToParty(const UObject *WorldContext,
                                                                      const FPokemonDTO &Pokemon) {
    auto Player = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>()->GetPlayer();
    check(Player != nullptr)
    auto CreatedPokemon = UnrealInjector::NewInjectedDependency<IPokemon>(WorldContext, Pokemon);
    Player->AddPokemonToParty(CreatedPokemon);
    return CreatedPokemon;
}