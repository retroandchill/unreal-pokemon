// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapSubsystem.generated.h"

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
	 * Play the specified sound as a musical jingle, pausing the background music and then resuming once it has completed.
	 * @param Jingle The jingle to play (will warn if null)
	 * @param VolumeMultiplier A linear scalar multiplied with the volume, in order to make the sound louder or softer.
	 * @param PitchMultiplier A linear scalar multiplied with the pitch.
	 */
	UFUNCTION(BlueprintCallable, Category = "Sound|Music")
	void PlayJingle(USoundBase* Jingle, float VolumeMultiplier = 1, float PitchMultiplier = 1);

	/**
	 * Is there currently background music that is playing?
	 * @return Is there music playing?
	 */
	UFUNCTION(BlueprintPure, Category = "Sound|Music")
	bool IsJinglePlaying() const;

	/**
	 * Warp to the given map name and coordinates
	 * @param MapName The name of the map to warp to
	 * @param X The X coordinate of the map to warp to
	 * @param Y The Y coordinate of the map to warp to
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Retain Direction)", Category = "Maps|Warping")
	void WarpToMap(FName MapName, int32 X, int32 Y);

	/**
	 * Warp to the given map name and coordinates
	 * @param MapName The name of the map to warp to
	 * @param X The X coordinate of the map to warp to
	 * @param Y The Y coordinate of the map to warp to
	 * @param Direction The direction the character should be facing after the warp
	 */
	UFUNCTION(BlueprintCallable, DisplayName = "Warp to Map (Change Direction)", Category = "Maps|Warping")
	void WarpToMapWithDirection(FName MapName, int32 X, int32 Y, EFacingDirection Direction);

	/**
	 * Set the location of the player in the world if there is a valid warp destination
	 * @param PlayerCharacter The character to set the location of
	 */
	void SetPlayerLocation(AGameCharacter* PlayerCharacter);
	
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
	TOptional<TTuple<int32, int32, EFacingDirection>> WarpDestination;
	
};
