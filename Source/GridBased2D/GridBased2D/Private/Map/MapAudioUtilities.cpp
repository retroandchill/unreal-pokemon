// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapAudioUtilities.h"

#include "Kismet/GameplayStatics.h"
#include "Map/MapSubsystem.h"
#include "Asserts.h"

void UMapAudioUtilities::PlayBackgroundMusic(UObject* WorldContext, USoundBase* BGM, float VolumeMultiplier,
                                             float PitchMultiplier) {
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)

	MapSubsystem->PlayBackgroundMusic(BGM, VolumeMultiplier, PitchMultiplier);
}

void UMapAudioUtilities::PauseBackgroundMusic(UObject* WorldContext) {
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)

	MapSubsystem->PauseBackgroundMusic();
}

void UMapAudioUtilities::ResumeBackgroundMusic(UObject* WorldContext) {
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)

	MapSubsystem->ResumeBackgroundMusic();
}

void UMapAudioUtilities::StopBackgroundMusic(UObject* WorldContext, float FadeOutDuration) {
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)

	MapSubsystem->StopBackgroundMusic(FadeOutDuration);
}

void UMapAudioUtilities::PlayJingle(UObject* WorldContext, USoundBase* Jingle, float VolumeMultiplier,
	float PitchMultiplier) {
	auto MapSubsystem = UGameplayStatics::GetGameInstance(WorldContext)->GetSubsystem<UMapSubsystem>();
	ASSERT(MapSubsystem != nullptr)

	MapSubsystem->PlayJingle(Jingle, VolumeMultiplier, PitchMultiplier);
}
