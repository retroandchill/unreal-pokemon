// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/TrainerCardScreen.h"
#include "Managers/PokemonSubsystem.h"
#include "Utilities/RPGMenuUtilities.h"
#include "Engine/GameInstance.h"

DEFINE_INJECTABLE_DEPENDENCY(UTrainerCardScreen)

UTrainerCardScreen *UTrainerCardScreen::AddTrainerCardScreenToStack(const UObject *WorldContextObject) {
    return URPGMenuUtilities::InjectScreenToStack<UTrainerCardScreen>(WorldContextObject).GetPtrOrNull();
}

void UTrainerCardScreen::NativeConstruct() {
    Super::NativeConstruct();

    auto PokemonSubsystem = GetGameInstance()->GetSubsystem<UPokemonSubsystem>();
    check(PokemonSubsystem != nullptr)
    PlayerTrainer = PokemonSubsystem->GetPlayer();
    PlayerMetadata = PokemonSubsystem->GetPlayerMetadata();
    OnPlayerInfoSet(PlayerTrainer, PlayerMetadata);
}