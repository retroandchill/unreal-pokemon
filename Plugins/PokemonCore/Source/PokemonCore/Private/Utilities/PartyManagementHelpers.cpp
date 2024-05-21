// "Unreal Pokémon" created by Retro & Chill.


#include "Utilities/PartyManagementHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/ConstructionUtilities.h"

TScriptInterface<IPokemon> UPartyManagementHelpers::AddPokemonToParty(const UObject *WorldContext, const FPokemonDTO &Pokemon) {
    auto Player = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>()->GetPlayer();
    check(Player != nullptr)
    auto CreatedPokemon = UConstructionUtilities::CreateNewPokemon(Pokemon);
    Player->AddPokemonToParty(CreatedPokemon);
    return CreatedPokemon;
}