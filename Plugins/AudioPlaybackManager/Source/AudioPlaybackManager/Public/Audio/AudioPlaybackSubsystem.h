// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioPlaybackSubsystem.generated.h"

/**
 * Subsystem used to handle the playback of audio to the player.
 */
UCLASS()
class AUDIOPLAYBACKMANAGER_API UAudioPlaybackSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
/**
     * Play the supplied audio file as the new BGM
     * @param BGM The new BGM to play. (Will be ignored if nullptr)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music")
    void PlayBackgroundMusic(USoundBase *BGM, float VolumeMultiplier = 1, float PitchMultiplier = 1);

    /**
     * Play the supplied audio file as the new BGM
     * @param BGM The new BGM to play. (Will be ignored if nullptr)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Play Background Music (Temporary Suspend)", Category = "Sound|Music")
    void PlayTempBackgroundMusic(USoundBase *BGM, float VolumeMultiplier = 1, float PitchMultiplier = 1);

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
     * Is there currently background music that is playing?
     * @return Is there music playing?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music")
    bool IsMusicPlaying() const;

    /**
     * Is there currently background music that is paused?
     * @return Is there paused background music?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music")
    bool IsMusicPaused() const;

    /**
     * Get the audio component for the currently playing music
     * @return The audio component for the background music
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music")
    UAudioComponent* GetCurrentMusic() const;

    /**
     * Play the specified sound as a musical jingle, pausing the background music and then resuming once it has
     * completed.
     * @param Jingle The jingle to play (will warn if null)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music")
    void PlayJingle(USoundBase *Jingle, float VolumeMultiplier = 1, float PitchMultiplier = 1);

    /**
     * Is there currently background music that is playing?
     * @return Is there music playing?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music")
    bool IsJinglePlaying() const;

private:
    /**
     * The currently playing background music component.
     */
    UPROPERTY()
    TObjectPtr<UAudioComponent> CurrentBackgroundMusic;

    /**
     * All suspected audio components. When the current background music is stopped the test takes
     * the next one off of the stack.
     */
    UPROPERTY()
    TArray<TObjectPtr<UAudioComponent>> SuspendedBackgroundMusic;

    /**
     * The currently playing musical jingle
     */
    UPROPERTY()
    TObjectPtr<UAudioComponent> CurrentJingle;

};
