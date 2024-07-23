// "Unreal Pokémon" created by Retro & Chill.


#include "Audio/AudioPlaybackSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UAudioPlaybackSubsystem::PlayBackgroundMusic(USoundBase *BGM, float VolumeMultiplier, float PitchMultiplier) {
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

void UAudioPlaybackSubsystem::PlayTempBackgroundMusic(USoundBase *BGM, float VolumeMultiplier, float PitchMultiplier) {
    if (BGM == nullptr) {
        return;
    }

    // Don't restart the music if its already playing
    if (CurrentBackgroundMusic != nullptr && CurrentBackgroundMusic->GetSound() == BGM) {
        return;
    }

    if (CurrentBackgroundMusic != nullptr) {
        CurrentBackgroundMusic->SetPaused(true);
        SuspendedBackgroundMusic.Emplace(CurrentBackgroundMusic);
    }

    CurrentBackgroundMusic =
        UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier, 0, nullptr, true);
}

void UAudioPlaybackSubsystem::PauseBackgroundMusic() {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }
    CurrentBackgroundMusic->SetPaused(true);
}

void UAudioPlaybackSubsystem::ResumeBackgroundMusic() {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }
    CurrentBackgroundMusic->SetPaused(false);
}

void UAudioPlaybackSubsystem::StopBackgroundMusic(float FadeOutDuration = 0) {
    if (CurrentBackgroundMusic == nullptr) {
        return;
    }
    
    CurrentBackgroundMusic->OnAudioFinishedNative.AddWeakLambda(this, [this](const UAudioComponent *) {
        if (!SuspendedBackgroundMusic.IsEmpty()) {
            CurrentBackgroundMusic = SuspendedBackgroundMusic.Pop();
            CurrentBackgroundMusic->SetPaused(IsJinglePlaying());
        }
    });

    if (FMath::IsNearlyZero(FadeOutDuration) || IsJinglePlaying()) {
        CurrentBackgroundMusic->Stop();
    } else {
        CurrentBackgroundMusic->FadeOut(FadeOutDuration, 0.f);
    }

    CurrentBackgroundMusic = nullptr;
}

bool UAudioPlaybackSubsystem::IsMusicPlaying() const {
    return CurrentBackgroundMusic != nullptr &&
           CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Playing;
}

bool UAudioPlaybackSubsystem::IsMusicPaused() const {
    return CurrentBackgroundMusic != nullptr &&
           CurrentBackgroundMusic->GetPlayState() == EAudioComponentPlayState::Paused;
}

UAudioComponent * UAudioPlaybackSubsystem::GetCurrentMusic() const {
    return CurrentBackgroundMusic;
}

void UAudioPlaybackSubsystem::PlayJingle(USoundBase *Jingle, float VolumeMultiplier, float PitchMultiplier) {
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

bool UAudioPlaybackSubsystem::IsJinglePlaying() const {
    return CurrentJingle != nullptr && CurrentJingle->GetPlayState() == EAudioComponentPlayState::Playing;
}