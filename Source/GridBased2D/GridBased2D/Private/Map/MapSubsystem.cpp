// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UMapSubsystem::PlayBackgroundMusic(USoundBase* BGM, float VolumeMultiplier, float PitchMultiplier) {
	if (BGM == nullptr) {
		UE_LOG(LogBlueprint, Warning, TEXT("Trying to play null for background music! Please specify an actual asset!"))
		return;
	}

	// Don't restart the music if its already playing
	if (CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetSound() == BGM) {
		return;
	}

	if (CurrentBackgroundMusic != nullptr) {
		CurrentBackgroundMusic->Stop();
	}

	CurrentBackgroundMusic = UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier,
		0, nullptr, true);
}

void UMapSubsystem::PauseBackgroundMusic() {
	if (CurrentBackgroundMusic == nullptr) {
		UE_LOG(LogBlueprint, Display, TEXT("Trying to pause background music, but none is currently playing!"))
		return;
	}

	CurrentBackgroundMusic->SetPaused(true);
}

void UMapSubsystem::ResumeBackgroundMusic() {
	if (CurrentBackgroundMusic == nullptr) {
		UE_LOG(LogBlueprint, Display, TEXT("Trying to resume background music, but none is currently playing!"))
		return;
	}

	CurrentBackgroundMusic->SetPaused(false);
}

void UMapSubsystem::StopBackgroundMusic(float FadeOutDuration = 0) {
	if (CurrentBackgroundMusic == nullptr) {
		UE_LOG(LogBlueprint, Display, TEXT("Trying to stop background music, but none is currently playing!"))
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
	return CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Playing;
}

bool UMapSubsystem::IsMusicPaused() const {
	return CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Paused;
}

void UMapSubsystem::PlayJingle(USoundBase* Jingle, float VolumeMultiplier, float PitchMultiplier) {
	if (Jingle == nullptr) {
		UE_LOG(LogBlueprint, Warning, TEXT("Trying to play a jingle, but the supplied sound was null!"))
		return;
	}

	if (CurrentJingle != nullptr) {
		UE_LOG(LogBlueprint, Warning, TEXT("Trying to play a jingle, but one is already playing!"))
		return;
	}

	PauseBackgroundMusic();
	CurrentJingle = UGameplayStatics::SpawnSound2D(this, Jingle, VolumeMultiplier, PitchMultiplier,
		0, nullptr, true);
	CurrentJingle->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Component) {
		CurrentJingle = nullptr;
		ResumeBackgroundMusic();
	});
}

bool UMapSubsystem::IsJinglePlaying() const {
	return CurrentJingle != nullptr && CurrentJingle->GetPlayState() == EAudioComponentPlayState::Playing;
}
