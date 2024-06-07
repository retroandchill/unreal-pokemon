// "Unreal Pokémon" created by Retro & Chill.

#include "Trainers/OwnerInfo.h"
#include "Managers/PokemonSubsystem.h"
#include "Trainers/Trainer.h"
#include "Trainers/TrainerType.h"

FOwnerInfo::FOwnerInfo() = default;

FOwnerInfo::FOwnerInfo(const UObject *WorldContext) {
    auto PlayerTrainer = UPokemonSubsystem::GetInstance(WorldContext).GetPlayer();
    if (PlayerTrainer == nullptr) {
        return;
    }

    OriginalTrainerName = PlayerTrainer->GetTrainerName();
    OriginalTrainerGender = PlayerTrainer->GetTrainerType().Gender;
    ID = PlayerTrainer->GetIdNumber();
    SecretID = PlayerTrainer->GetSecretId();
}

FOwnerInfo::FOwnerInfo(const ITrainer &Trainer)
    : OriginalTrainerName(Trainer.GetTrainerName()), OriginalTrainerGender(Trainer.GetTrainerType().Gender),
      ID(Trainer.GetIdNumber()), SecretID(Trainer.GetSecretId()) {
}