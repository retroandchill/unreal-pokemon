// "Unreal Pokémon" created by Retro & Chill.
#include "Components/Party/PokemonSelectionPane.h"
#include "Managers/PokemonSubsystem.h"
#include "PokemonDataSettings.h"

void UPokemonSelectionPane::NativeConstruct() {
    Super::NativeConstruct();

    const auto &PokemonSubsystem = UPokemonSubsystem::GetInstance(this);
    auto &PlayerParty = PokemonSubsystem.GetPlayer()->GetParty();
    SetPokemonToDisplay(PlayerParty);
}

TOptional<int32> UPokemonSelectionPane::GetNumPanelsToAdd() const {
    return GetDefault<UPokemonDataSettings>()->MaxPartySize;
}
