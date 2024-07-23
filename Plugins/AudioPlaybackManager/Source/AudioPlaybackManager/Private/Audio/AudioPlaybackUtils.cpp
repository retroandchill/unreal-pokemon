// "Unreal Pokémon" created by Retro & Chill.


#include "Audio/AudioPlaybackUtils.h"
#include "Audio/AudioPlaybackSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UAudioPlaybackUtils::PlayBackgroundMusic(const UObject* WorldContext, USoundBase *BGM, float VolumeMultiplier,
                                              float PitchMultiplier) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->PlayBackgroundMusic(BGM, VolumeMultiplier, PitchMultiplier);
}

void UAudioPlaybackUtils::PlayTempBackgroundMusic(const UObject* WorldContext, USoundBase *BGM, float VolumeMultiplier,
                                                  float PitchMultiplier) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->PlayTempBackgroundMusic(BGM, VolumeMultiplier, PitchMultiplier);
}

void UAudioPlaybackUtils::PauseBackgroundMusic(const UObject* WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->PauseBackgroundMusic();
}

void UAudioPlaybackUtils::ResumeBackgroundMusic(const UObject* WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->ResumeBackgroundMusic();
}

void UAudioPlaybackUtils::StopBackgroundMusic(const UObject* WorldContext, float FadeOutDuration) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->StopBackgroundMusic(FadeOutDuration);
}

bool UAudioPlaybackUtils::IsMusicPlaying(const UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)
    return MapSubsystem->IsMusicPlaying();
}

bool UAudioPlaybackUtils::IsMusicPaused(const UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)
    return MapSubsystem->IsMusicPaused();
}

void UAudioPlaybackUtils::PlayJingle(const UObject* WorldContext, USoundBase *Jingle, float VolumeMultiplier,
                                     float PitchMultiplier) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)

    MapSubsystem->PlayJingle(Jingle, VolumeMultiplier, PitchMultiplier);
}

bool UAudioPlaybackUtils::IsJinglePlaying(const UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UAudioPlaybackSubsystem>();
    check(MapSubsystem != nullptr)
    return MapSubsystem->IsJinglePlaying();
}
