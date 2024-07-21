// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioPlaybackManagerSubsystem.generated.h"

USTRUCT()
struct AUDIOPLAYBACKMANAGER_API FAudioStack {
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<UAudioComponent>> Data;
};

/**
 * Subsystem for handling the audio playback 
 */
UCLASS()
class AUDIOPLAYBACKMANAGER_API UAudioPlaybackManagerSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = Audio)
    void PlayAudioOnChannel(FName Channel, USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);

    UFUNCTION(BlueprintCallable, Category = Audio)
    void PlayTemporaryAudioOnChannel(FName Channel, USoundBase* Sound, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f);
    
    UFUNCTION(BlueprintCallable, Category = Audio)
    void StopCurrentAudioOnChannel(FName Channel);
    

private:
    TMap<FName, FAudioStack> AudioChannels;

};
