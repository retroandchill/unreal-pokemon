// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapSubsystem.h"

#include "Asserts.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UMapSubsystem::PlayBackgroundMusic(USoundBase* BGM, float VolumeMultiplier, float PitchMultiplier) {
	GUARD_WARN(BGM == nullptr, ,TEXT("Trying to play null for background music! Please specify an actual asset!"))

	// Don't restart the music if its already playing
	GUARD(CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetSound() == BGM, )

	if (CurrentBackgroundMusic != nullptr) {
		CurrentBackgroundMusic->Stop();
	}

	CurrentBackgroundMusic = UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier,
		0, nullptr, true);
}

void UMapSubsystem::PauseBackgroundMusic() {
	GUARD(CurrentBackgroundMusic == nullptr, )
	CurrentBackgroundMusic->SetPaused(true);
}

void UMapSubsystem::ResumeBackgroundMusic() {
	GUARD(CurrentBackgroundMusic == nullptr, )
	CurrentBackgroundMusic->SetPaused(false);
}

void UMapSubsystem::StopBackgroundMusic(float FadeOutDuration = 0) {
	GUARD(CurrentBackgroundMusic == nullptr, )
	
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
	GUARD_WARN(Jingle == nullptr, ,TEXT("Trying to play a jingle, but the supplied sound was null!"))
	GUARD_WARN(CurrentJingle != nullptr, ,TEXT("Trying to play a jingle, but one is already playing!"))

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
