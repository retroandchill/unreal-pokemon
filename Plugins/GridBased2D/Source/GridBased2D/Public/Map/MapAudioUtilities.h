// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "MapAudioUtilities.generated.h"

/**
 * Blueprint Function Library used to handle functions related to playing Audio on the map
 */
UCLASS()
class GRIDBASED2D_API UMapAudioUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Play the supplied audio file as the new BGM
     * @param WorldContext Object used to get the Map Subsystem
     * @param BGM The new BGM to play. (Will be ignored if nullptr)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music",
              meta = (WorldContext = WorldContext, AdvancedDisplay = "VolumeMultiplier,PitchMultiplier"))
    static void PlayBackgroundMusic(UObject *WorldContext, USoundBase *BGM, float VolumeMultiplier = 1,
                                    float PitchMultiplier = 1);

    /**
     * Pause the currently playing BGM
     * @param WorldContext Object used to get the Map Subsystem
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music", meta = (WorldContext = WorldContext))
    static void PauseBackgroundMusic(UObject *WorldContext);

    /**
     * Resume the paused BGM
     * @param WorldContext Object used to get the Map Subsystem
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music", meta = (WorldContext = WorldContext))
    static void ResumeBackgroundMusic(UObject *WorldContext);

    /**
     * Stop the currently playing BGM
     * @param WorldContext Object used to get the Map Subsystem
     * @param FadeOutDuration The amount of time it should take to fade out
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music",
              meta = (WorldContext = WorldContext, AdvancedDisplay = "FadeOutDuration"))
    static void StopBackgroundMusic(UObject *WorldContext, float FadeOutDuration = 0);

    /**
     * Is there currently background music that is playing?
     * @param WorldContext Object used to get the Map Subsystem
     * @return Is there music playing?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music", meta = (WorldContext = WorldContext))
    static bool IsMusicPlaying(UObject *WorldContext);

    /**
     * Is there currently background music that is paused?
     * @param WorldContext Object used to get the Map Subsystem
     * @return Is there paused background music?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music", meta = (WorldContext = WorldContext))
    static bool IsMusicPaused(UObject *WorldContext);

    /**
     * Play the specified sound as a musical jingle, pausing the background music and then resuming once it has
     * completed.
     * @param Jingle The jingle to play (will warn if null)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music",
              meta = (WorldContext = WorldContext, AdvancedDisplay = "VolumeMultiplier,PitchMultiplier"))
    static void PlayJingle(UObject *WorldContext, USoundBase *Jingle, float VolumeMultiplier = 1,
                           float PitchMultiplier = 1);

    /**
     * Is there currently background music that is playing?
     * @param WorldContext Object used to get the Map Subsystem
     * @return Is there music playing?
     */
    UFUNCTION(BlueprintPure, Category = "Sound|Music", meta = (WorldContext = WorldContext))
    static bool IsJinglePlaying(UObject *WorldContext);
};
