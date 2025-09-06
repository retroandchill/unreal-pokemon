// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/TrainerHelpers.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/PlayerMetadata.h"

const TScriptInterface<ITrainer> &UTrainerHelpers::GetPlayerCharacter(const UObject *WorldContextObject)
{
    return UPokemonSubsystem::GetInstance(WorldContextObject).GetPlayer();
}

const TScriptInterface<IBag> &UTrainerHelpers::GetBag(const UObject *WorldContextObject)
{
    return UPokemonSubsystem::GetInstance(WorldContextObject).GetBag();
}

bool UTrainerHelpers::IsRepelActive(const UObject *WorldContextObject)
{
    auto PlayerMetadata = UPokemonSubsystem::GetInstance(WorldContextObject).GetPlayerMetadata();
    check(PlayerMetadata != nullptr)
    return PlayerMetadata->GetRepelSteps() > 0;
}

bool UTrainerHelpers::ApplyRepel(const UObject *WorldContextObject, int32 Steps)
{
    auto PlayerMetadata = UPokemonSubsystem::GetInstance(WorldContextObject).GetPlayerMetadata();
    check(PlayerMetadata != nullptr)
    if (PlayerMetadata->GetRepelSteps() > 0)
    {
        return false;
    }

    PlayerMetadata->SetRepelSteps(Steps);
    return true;
}

void UTrainerHelpers::DecrementRepelSteps(const UObject *WorldContextObject)
{
    auto PlayerMetadata = UPokemonSubsystem::GetInstance(WorldContextObject).GetPlayerMetadata();
    check(PlayerMetadata != nullptr)
    if (PlayerMetadata->GetRepelSteps() > 0)
    {
        return PlayerMetadata->SetRepelSteps(PlayerMetadata->GetRepelSteps() - 1);
    }
}