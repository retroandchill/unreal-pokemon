// "Unreal Pokémon" created by Retro & Chill.

#include "Map/MapAudioUtilities.h"

#include "Kismet/GameplayStatics.h"
#include "Map/MapSubsystem.h"

void UMapAudioUtilities::PlayBackgroundMusic(UObject *WorldContext, USoundBase *BGM, float VolumeMultiplier,
                                             float PitchMultiplier) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)

        MapSubsystem->PlayBackgroundMusic(BGM, VolumeMultiplier, PitchMultiplier);
}

void UMapAudioUtilities::PauseBackgroundMusic(UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)

        MapSubsystem->PauseBackgroundMusic();
}

void UMapAudioUtilities::ResumeBackgroundMusic(UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)

        MapSubsystem->ResumeBackgroundMusic();
}

void UMapAudioUtilities::StopBackgroundMusic(UObject *WorldContext, float FadeOutDuration) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)

        MapSubsystem->StopBackgroundMusic(FadeOutDuration);
}

bool UMapAudioUtilities::IsMusicPlaying(UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)
return MapSubsystem->IsMusicPlaying();
}

bool UMapAudioUtilities::IsMusicPaused(UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)
return MapSubsystem->IsMusicPaused();
}

void UMapAudioUtilities::PlayJingle(UObject *WorldContext, USoundBase *Jingle, float VolumeMultiplier,
                                    float PitchMultiplier) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)

        MapSubsystem->PlayJingle(Jingle, VolumeMultiplier, PitchMultiplier);
}

bool UMapAudioUtilities::IsJinglePlaying(UObject *WorldContext) {
    auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
    check(MapSubsystem != nullptr)
return MapSubsystem->IsJinglePlaying();
}
