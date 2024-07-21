// "Unreal Pokémon" created by Retro & Chill.

#include "Map/MapSubsystem.h"
#include "EngineUtils.h"
#include "GridBased2D.h"
#include "Components/AudioComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"
#include "Engine/LevelStreamingDynamic.h"
#include "GridBased2DSettings.h"
#include "RangeHelpers.h"
#include "Characters/GamePlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Map/TileMapGridBasedMap.h"
#include <range/v3/view/filter.hpp>

DECLARE_DELEGATE(FLoadFinalized)

UMapSubsystem::UMapSubsystem(const FObjectInitializer &) {
}

void UMapSubsystem::PlayBackgroundMusic(USoundBase *BGM, float VolumeMultiplier, float PitchMultiplier) {
    if (BGM == nullptr) {
        return;
    }

    // Don't restart the music if its already playing
    if (CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetSound() == BGM) {
        return;
    }

    if (CurrentBackgroundMusic != nullptr) {
        CurrentBackgroundMusic->Stop();
    }

    CurrentBackgroundMusic =
        UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier, 0, nullptr, true);
}

void UMapSubsystem::PauseBackgroundMusic() {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }
    CurrentBackgroundMusic->SetPaused(true);
}

void UMapSubsystem::ResumeBackgroundMusic() {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }
    CurrentBackgroundMusic->SetPaused(false);
}

void UMapSubsystem::StopBackgroundMusic(float FadeOutDuration = 0) {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }

    if (FMath::IsNearlyZero(FadeOutDuration)) {
        CurrentBackgroundMusic->Stop();
    } else {
        CurrentBackgroundMusic->FadeOut(FadeOutDuration, 0.f);
    }

    CurrentBackgroundMusic = nullptr;
}

bool UMapSubsystem::IsMusicPlaying() const {
    return CurrentBackgroundMusic != nullptr &&
           CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Playing;
}

bool UMapSubsystem::IsMusicPaused() const {
    return CurrentBackgroundMusic != nullptr &&
           CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Paused;
}

void UMapSubsystem::PlayJingle(USoundBase *Jingle, float VolumeMultiplier, float PitchMultiplier) {
    if (Jingle == nullptr) {
        return;
    }
    if (CurrentJingle != nullptr) {
        return;
    }

    PauseBackgroundMusic();
    CurrentJingle = UGameplayStatics::SpawnSound2D(this, Jingle, VolumeMultiplier, PitchMultiplier, 0, nullptr, true);
    CurrentJingle->OnAudioFinishedNative.AddLambda([this](const UAudioComponent *const) {
        CurrentJingle = nullptr;
        ResumeBackgroundMusic();
    });
}

bool UMapSubsystem::IsJinglePlaying() const {
    return CurrentJingle != nullptr && CurrentJingle->GetPlayState() == EAudioComponentPlayState::Playing;
}

void UMapSubsystem::WarpToMap(const TSoftObjectPtr<UWorld> &Map, FName WarpTag) {
    auto PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();
    if (PlayerCharacter == nullptr) {
        UE_LOG(LogBlueprint, Warning, TEXT("There is no valid pawn!"))
        return;
    }

    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter);
    if (MovementComponent == nullptr) {
        UE_LOG(LogBlueprint, Warning, TEXT("The pawn class does not implement IGridMovable!"))
        return;
    }
    WarpToMapWithDirection(Map, WarpTag, MovementComponent->GetDirection());
}

void UMapSubsystem::WarpToMapWithDirection(const TSoftObjectPtr<UWorld> &Map, FName WarpTag,
                                           EFacingDirection Direction) {
    WarpDestination.Emplace(WarpTag, Direction);
    UGameplayStatics::OpenLevelBySoftObjectPtr(this, Map);
}

void UMapSubsystem::SetPlayerLocation(const TScriptInterface<IGridMovable> &PlayerCharacter) {
    if (!WarpDestination.IsSet()) {
        return;
    }

    auto [WarpTag, Direction] = WarpDestination.GetValue();
    auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter.GetObject());
    MovementComponent->FaceDirection(Direction);
    WarpDestination.Reset();
}

void UMapSubsystem::UpdateCharacterMapPosition(const TScriptInterface<IGridMovable> &Movable) const {
    TArray<AActor *> Maps;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UMapGrid::StaticClass(), Maps);
    TScriptInterface<IMapGrid> OldMap = nullptr;
    TScriptInterface<IMapGrid> NewMap = nullptr;
    for (auto Actor : Maps) {
        TScriptInterface<IMapGrid> Map = Actor;
        if (OldMap == nullptr && Map->IsCharacterPartOfMap(Movable)) {
            OldMap = Map;
        }

        if (auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(Movable.GetObject());
            NewMap == nullptr && Map->IsPositionInMap(MovementComponent->GetCurrentPosition())) {
            NewMap = Map;
        }

        if (OldMap != nullptr && NewMap != nullptr) {
            break;
        }
    }

    if (OldMap == NewMap) {
        return;
    }

    if (OldMap != nullptr) {
        OldMap->RemoveCharacter(Movable);
    }

    if (NewMap != nullptr) {
        NewMap->AddCharacter(Movable);
    }
}

const TOptional<TPair<FName, EFacingDirection>> & UMapSubsystem::GetWarpDestination() const {
    return WarpDestination;
}
