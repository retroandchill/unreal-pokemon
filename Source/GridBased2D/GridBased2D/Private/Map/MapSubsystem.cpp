// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapSubsystem.h"

#include "Asserts.h"
#include "GridBased2DSettings.h"
#include "GridUtils.h"
#include "Map/GridBasedMap.h"
#include "Components/AudioComponent.h"
#include "Components/GridBasedMovementComponent.h"
#include "Components/GridMovable.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Kismet/GameplayStatics.h"

DECLARE_DELEGATE(FLoadFinalized)

UMapSubsystem::UMapSubsystem(const FObjectInitializer&) :
	DynamicLevelOffset(GetDefault<UGridBased2DSettings>()->GetDynamicLevelOffset()) {
}

void UMapSubsystem::PlayBackgroundMusic(USoundBase* BGM, float VolumeMultiplier, float PitchMultiplier) {
	GUARD_WARN(BGM == nullptr, , TEXT("Trying to play null for background music! Please specify an actual asset!"))

	// Don't restart the music if its already playing
	GUARD(CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetSound() == BGM,)

	if (CurrentBackgroundMusic != nullptr) {
		CurrentBackgroundMusic->Stop();
	}

	CurrentBackgroundMusic = UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier,
	                                                        0, nullptr, true);
}

void UMapSubsystem::PauseBackgroundMusic() {
	GUARD(CurrentBackgroundMusic == nullptr,)
	CurrentBackgroundMusic->SetPaused(true);
}

void UMapSubsystem::ResumeBackgroundMusic() {
	GUARD(CurrentBackgroundMusic == nullptr,)
	CurrentBackgroundMusic->SetPaused(false);
}

void UMapSubsystem::StopBackgroundMusic(float FadeOutDuration = 0) {
	GUARD(CurrentBackgroundMusic == nullptr,)

	if (FMath::IsNearlyZero(FadeOutDuration)) {
		CurrentBackgroundMusic->Stop();
	} else {
		CurrentBackgroundMusic->FadeOut(FadeOutDuration, 0.f);
	}

	CurrentBackgroundMusic = nullptr;
}

bool UMapSubsystem::IsMusicPlaying() const {
	return CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetPlayState() ==
		EAudioComponentPlayState::Playing;
}

bool UMapSubsystem::IsMusicPaused() const {
	return CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetPlayState() ==
		EAudioComponentPlayState::Paused;
}

void UMapSubsystem::PlayJingle(USoundBase* Jingle, float VolumeMultiplier, float PitchMultiplier) {
	GUARD_WARN(Jingle == nullptr, , TEXT("Trying to play a jingle, but the supplied sound was null!"))
	GUARD_WARN(CurrentJingle != nullptr, , TEXT("Trying to play a jingle, but one is already playing!"))

	PauseBackgroundMusic();
	CurrentJingle = UGameplayStatics::SpawnSound2D(this, Jingle, VolumeMultiplier, PitchMultiplier,
	                                               0, nullptr, true);
	CurrentJingle->OnAudioFinishedNative.AddLambda([this](const UAudioComponent* const) {
		CurrentJingle = nullptr;
		ResumeBackgroundMusic();
	});
}

bool UMapSubsystem::IsJinglePlaying() const {
	return CurrentJingle != nullptr && CurrentJingle->GetPlayState() == EAudioComponentPlayState::Playing;
}

void UMapSubsystem::WarpToMap(TSoftObjectPtr<UWorld> Map, int32 X, int32 Y) {
	auto PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();
	GUARD_WARN(PlayerCharacter == nullptr, , TEXT("There is no valid pawn!"))

	auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter);
	GUARD_WARN(MovementComponent == nullptr, , TEXT("The pawn class does not implement IGridMovable!"))
	WarpToMapWithDirection(Map, X, Y, MovementComponent->GetDirection());
}

void UMapSubsystem::WarpToMapWithDirection(TSoftObjectPtr<UWorld> Map, int32 X, int32 Y, EFacingDirection Direction) {
	if (DynamicallyStreamedLevel != nullptr) {
		FLatentActionInfo LatentActionInfo;
		UGameplayStatics::UnloadStreamLevelBySoftObjectPtr(this, DynamicallyStreamedLevel->GetWorldAsset(),
														   LatentActionInfo, false);
	}
	
	if (auto StreamedLevel = GetWorld()->GetStreamingLevels().FindByPredicate([&Map](const ULevelStreaming* Level) {
		return Level->GetWorldAsset().GetUniqueID() == Map.GetUniqueID();
	}); StreamedLevel != nullptr) {
		(*StreamedLevel)->OnLevelShown.Clear();
		(*StreamedLevel)->OnLevelShown.AddDynamic(this, &UMapSubsystem::UMapSubsystem::UpdatePlayerCharacterPosition);
		WarpDestination.Emplace((*StreamedLevel)->LevelTransform.GetLocation(), X, Y, Direction);
		OnNewLevelLoaded();
	} else {
		bool bSuccess;
		DynamicallyStreamedLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(
			this, Map, DynamicLevelOffset, FRotator(), bSuccess);
		DynamicallyStreamedLevel->OnLevelShown.AddDynamic(this, &UMapSubsystem::OnNewLevelLoaded);
		WarpDestination.Emplace(DynamicLevelOffset, X, Y, Direction);
	}
}

void UMapSubsystem::SetPlayerLocation(const TScriptInterface<IGridMovable>& PlayerCharacter) {
	GUARD(!WarpDestination.IsSet(),)

	auto [Offset, X, Y, Direction] = WarpDestination.GetValue();
	auto MovementComponent = IGridMovable::Execute_GetGridBasedMovementComponent(PlayerCharacter.GetObject());
	MovementComponent->WarpToLocation(X, Y, Offset);
	MovementComponent->FaceDirection(Direction);
	WarpDestination.Reset();
}

void UMapSubsystem::UpdateCharacterMapPosition(const TScriptInterface<IGridMovable>& Movable) {
	TArray<AActor*> Maps;
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

	GUARD(OldMap == NewMap,)

	if (OldMap != nullptr) {
		OldMap->RemoveCharacter(Movable);
	}

	if (NewMap != nullptr) {
		NewMap->AddCharacter(Movable);
	}
}

void UMapSubsystem::OnNewLevelLoaded() {
	auto PlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);
	ASSERT(PlayerCharacter != nullptr && PlayerCharacter->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
	SetPlayerLocation(PlayerCharacter);
	UpdateCharacterMapPosition(PlayerCharacter);
}

void UMapSubsystem::UpdatePlayerCharacterPosition() {
	auto PlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);
	ASSERT(PlayerCharacter != nullptr && PlayerCharacter->GetClass()->ImplementsInterface(UGridMovable::StaticClass()))
	UpdateCharacterMapPosition(PlayerCharacter);
}
