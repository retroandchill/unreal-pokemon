// "Unreal Pokémon" created by Retro & Chill.


#include "Map/MapSubsystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UMapSubsystem::PlayBackgroundMusic(USoundBase* BGM, float VolumeMultiplier, float PitchMultiplier) {
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

	CurrentBackgroundMusic = UGameplayStatics::SpawnSound2D(this, BGM, VolumeMultiplier, PitchMultiplier,
		0, nullptr, true);
}
