// "Unreal Pokémon" created by Retro & Chill.


#include "Player/RPGLocalPlayer.h"
#include "GameUIPolicy.h"
#include "RPGUIManagerSubsystem.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "PrimaryGameLayout.h"

FDelegateHandle URPGLocalPlayer::RegisterOnPlayerControllerSet(FPlayerControllerSetDelegate::FDelegate Delegate) {
    if (auto Controller = GetPlayerController(GetWorld()); Controller != nullptr) {
        Delegate.Execute(this, Controller);
    }

    return OnPlayerControllerSet.Add(std::move(Delegate));
}

FDelegateHandle URPGLocalPlayer::RegisterOnPlayerStateSet(FPlayerStateSetDelegate::FDelegate Delegate) {
    auto Controller = GetPlayerController(GetWorld());
    if (APlayerState *PlayerState = Controller != nullptr ? Controller->PlayerState : nullptr; PlayerState != nullptr) {
        Delegate.Execute(this, PlayerState);
    }

    return OnPlayerStateSet.Add(std::move(Delegate));
}

FDelegateHandle URPGLocalPlayer::RegisterOnPlayerPawnSet(FPlayerPawnSetDelegate::FDelegate Delegate) {
    auto Controller = GetPlayerController(GetWorld());
    if (auto Pawn = Controller != nullptr ? Controller->GetPawn() : nullptr; Pawn != nullptr) {
        Delegate.Execute(this, Pawn);
    }

    return OnPlayerPawnSet.Add(std::move(Delegate));
}

bool URPGLocalPlayer::GetProjectionData(FViewport *Viewport, FSceneViewProjectionData &ProjectionData,
                                        int32 StereoViewIndex) const {
    if (!bIsPlayerViewEnabled) {
        return false;
    }

    return Super::GetProjectionData(Viewport, ProjectionData, StereoViewIndex);
}

UPrimaryGameLayout *URPGLocalPlayer::GetRootUILayout() const {
    // clang-format off
    return Retro::Optionals::OfNullable(GetGameInstance()->GetSubsystem<URPGUIManagerSubsystem>()) |
           Retro::Optionals::Transform(RPG::Menus::GetCurrentUIPolicy) |
           Retro::Optionals::Transform(Retro::BindBack<&UGameUIPolicy::GetRootLayout>(this)) |
           Retro::Optionals::PtrOrNull;
    // clang-format on
}