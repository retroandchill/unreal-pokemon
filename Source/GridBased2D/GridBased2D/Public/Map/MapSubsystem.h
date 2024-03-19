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

	/**
	 * Pause the currently playing BGM
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void PauseBackgroundMusic();

	/**
	 * Resume the paused BGM
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void ResumeBackgroundMusic();

	/**
	 * Stop the currently playing BGM
	 * @param FadeOutDuration The amount of time it should take to fade out
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void StopBackgroundMusic(float FadeOutDuration);

	/**
	 * Play the specified sound as a musical jingle, pausing the background music and then resuming once it has completed.
	 * @param Jingle The jingle to play (will warn if null)
	 * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier A linear scalar multiplied with the pitch.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void PlayJingle(USoundBase* Jingle, float VolumeMultiplier = 1, float PitchMultiplier = 1);
	
private:
	/**
	 * The currently playing background music component.
	 */
	UPROPERTY()
	TObjectPtr<UAudioComponent> CurrentBackgroundMusic;

	/**
	 * The currently playing musical jingle
	 */
	UPROPERTY()
	TObjectPtr<UAudioComponent> CurrentJingle;
};
