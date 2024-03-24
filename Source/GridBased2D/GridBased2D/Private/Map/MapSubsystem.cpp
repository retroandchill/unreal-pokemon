// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapSubsystem.h"

#include "Asserts.h"
#include "GridBased2DSettings.h"
#include "GridUtils.h"
#include "Characters/GameCharacter.h"
#include "Map/GridBasedMap.h"
#include "Components/AudioComponent.h"
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
	auto PlayerCharacter = Cast<AGameCharacter>(GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator());
	GUARD_WARN(PlayerCharacter == nullptr, , TEXT("The player character is not an instance of AGameCharacter!"))

	WarpToMapWithDirection(Map, X, Y, PlayerCharacter->GetDirection());
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

void UMapSubsystem::SetPlayerLocation(AGameCharacter* PlayerCharacter) {
	GUARD(!WarpDestination.IsSet(),)

	auto [Offset, X, Y, Direction] = WarpDestination.GetValue();
	PlayerCharacter->WarpToLocation(X, Y, Offset);
	PlayerCharacter->FaceDirection(Direction);
	WarpDestination.Reset();
}

void UMapSubsystem::UpdateCharacterMapPosition(AGameCharacter* Character) {
	auto Maps = UGridUtils::FindAllActors<AGridBasedMap>(Character);
	AGridBasedMap* OldMap = nullptr;
	AGridBasedMap* NewMap = nullptr;
	for (auto Map : Maps) {
		if (OldMap == nullptr && Map->IsCharacterPartOfMap(Character)) {
			OldMap = Map;
		}

		if (NewMap == nullptr && Map->IsPositionInMap(Character->GetCurrentPosition())) {
			NewMap = Map;
		}

		if (OldMap != nullptr && NewMap != nullptr) {
			break;
		}
	}

	GUARD(OldMap == NewMap,)

	if (OldMap != nullptr) {
		OldMap->RemoveCharacter(Character);
	}

	if (NewMap != nullptr) {
		NewMap->AddCharacter(Character);
	}
}

void UMapSubsystem::OnNewLevelLoaded() {
	auto PlayerCharacter = Cast<AGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	ASSERT(PlayerCharacter != nullptr)
	SetPlayerLocation(PlayerCharacter);
	UpdateCharacterMapPosition(PlayerCharacter);
}

void UMapSubsystem::UpdatePlayerCharacterPosition() {
	auto PlayerCharacter = Cast<AGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	ASSERT(PlayerCharacter != nullptr)
	UpdateCharacterMapPosition(PlayerCharacter);
}
