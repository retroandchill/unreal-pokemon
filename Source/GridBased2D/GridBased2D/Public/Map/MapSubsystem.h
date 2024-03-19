// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSubsystem.generated.h"

/**
 * Subsystem that handles the traversal between various maps as well as the music within a given map.
 */
UCLASS()
class GRIDBASED2D_API UMapSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	/**
	 * Play the supplied audio file as the new BGM
	 * @param BGM The new BGM to play. (Will be ignored if nullptr)
	 * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier A linear scalar multiplied with the pitch.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void PlayBackgroundMusic(USoundBase* BGM, float VolumeMultiplier = 1, float PitchMultiplier = 1);
	
private:
	/**
	 * The currently playing background music component.
	 */
	UPROPERTY()
	TObjectPtr<UAudioComponent> CurrentBackgroundMusic;
};
