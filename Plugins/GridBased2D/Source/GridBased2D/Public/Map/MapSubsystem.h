// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridBased2DSettings.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "MapSubsystem.generated.h"

class IMapGrid;
class IGridMovable;
class UGridBasedMovementComponent;
class ULevelStreamingDynamic;
class AGameCharacter;
enum class EFacingDirection : uint8;
class IInteractable;

/**
 * Subsystem that handles the traversal between various maps as well as the music within a given map.
 */
UCLASS()
class GRIDBASED2D_API UMapSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    /**
     * Initialize the default object
     * @param Initializer The Unreal initializer
     */
    explicit UMapSubsystem(const FObjectInitializer &Initializer);

    /**
     * Play the supplied audio file as the new BGM
     * @param BGM The new BGM to play. (Will be ignored if nullptr)
     * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
     * @param PitchMultiplier A linear scalar multiplied with the pitch.
     */
    UFUNCTION(BlueprintCallable, Category = "Sound|Music")
    void PlayBackgroundMusic(USoundBase *BGM, float VolumeMultiplier = 1, float PitchMultiplier = 1);

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

    /**
     * Warp to the given map name and coordinates
     * @param Map The the map to warp to
     * @param WarpTag The X coordinate of the map to warp to
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Retain Direction)", Category = "Maps|Warping")
    void WarpToMap(const TSoftObjectPtr<UWorld> &Map, FName WarpTag);

    /**
     * Warp to the given map name and coordinates
     * @param Map The the map to warp to
     * @param WarpTag The X coordinate of the map to warp to
     * @param Direction The direction the character should be facing after the warp
     */
    UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Change Direction)", Category = "Maps|Warping")
    void WarpToMapWithDirection(const TSoftObjectPtr<UWorld> &Map, FName WarpTag, EFacingDirection Direction);

    /**
     * Set the location of the player in the world if there is a valid warp destination
     * @param PlayerCharacter The character to set the location of
     */
    void SetPlayerLocation(const TScriptInterface<IGridMovable> &PlayerCharacter);

    /**
     * Update what map the player is considered to be a part of
     * @param Movable The movement object in question
     */
    void UpdateCharacterMapPosition(const TScriptInterface<IGridMovable> &Movable) const;

    const TOptional<TPair<FName, EFacingDirection>> &GetWarpDestination() const;

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

    /**
     * If set, indicates that the player warping to another location
     */
    TOptional<TPair<FName, EFacingDirection>> WarpDestination;
};
