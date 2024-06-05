// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/TrainerHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/PokemonSubsystem.h"

const TScriptInterface<ITrainer> &UTrainerHelpers::GetPlayerCharacter(const UObject *WorldContext) {
    return UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>()->GetPlayer();
}

const TScriptInterface<IBag> & UTrainerHelpers::GetBag(const UObject *WorldContext) {
    return UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UPokemonSubsystem>()->GetBag();
}