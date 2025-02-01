// "Unreal Pokémon" created by Retro & Chill.

#include "RPGPlayerController.h"
#include "Player/RPGLocalPlayer.h"

void ARPGPlayerController::ReceivedPlayer() {
    Super::ReceivedPlayer();

    if (auto LocalPlayer = Cast<URPGLocalPlayer>(Player); LocalPlayer != nullptr) {
        LocalPlayer->OnPlayerControllerSet.Broadcast(LocalPlayer, this);

        if (PlayerState != nullptr) {
            LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
        }
    }
}

void ARPGPlayerController::SetPawn(APawn *InPawn) {
    Super::SetPawn(InPawn);

    if (auto LocalPlayer = Cast<URPGLocalPlayer>(Player); LocalPlayer != nullptr) {
        LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, InPawn);
    }
}

void ARPGPlayerController::OnPossess(APawn *APawn) {
    Super::OnPossess(APawn);

    if (auto LocalPlayer = Cast<URPGLocalPlayer>(Player); LocalPlayer != nullptr) {
        LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, APawn);
    }
}

void ARPGPlayerController::OnUnPossess() {
    Super::OnUnPossess();

    if (auto LocalPlayer = Cast<URPGLocalPlayer>(Player); LocalPlayer != nullptr) {
        LocalPlayer->OnPlayerPawnSet.Broadcast(LocalPlayer, nullptr);
    }
}

void ARPGPlayerController::OnRep_PlayerState() {
    Super::OnRep_PlayerState();

    if (PlayerState != nullptr) {
        if (auto LocalPlayer = Cast<URPGLocalPlayer>(Player); LocalPlayer != nullptr) {
            LocalPlayer->OnPlayerStateSet.Broadcast(LocalPlayer, PlayerState);
        }
    }
}