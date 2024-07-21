// "Unreal Pokémon" created by Retro & Chill.


#include "AudioPlaybackManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

template <typename F>
static void RemoveTopOfStack(TArray<TObjectPtr<UAudioComponent>>& ChannelStack, const F& Callback) {
    ChannelStack.Top()->Stop();
    ChannelStack.Pop();
    Callback(ChannelStack[ChannelStack.Num() - 1]);
}

void UAudioPlaybackManagerSubsystem::PlayAudioOnChannel(FName Channel, USoundBase *Sound, float VolumeMultiplier,
                                                        float PitchMultiplier) {
    auto &ChannelStack = AudioChannels.FindOrAdd(Channel).Data;
    if (!ChannelStack.IsEmpty()) {
        RemoveTopOfStack(ChannelStack, [](UAudioComponent*){});
    }

    auto AudioComponent = UGameplayStatics::SpawnSound2D(this, Sound, VolumeMultiplier, PitchMultiplier, 0, nullptr, true);
    AudioComponent->OnAudioFinishedNative.AddWeakLambda(this, [this, Channel](UAudioComponent* Component) {
        if (auto &FoundStack = AudioChannels.FindOrAdd(Channel).Data; !FoundStack.IsEmpty() && FoundStack.Top() == Component) {
            RemoveTopOfStack(FoundStack, [](UAudioComponent* Element) { Element->SetPaused(false); });
        }
    });
    ChannelStack.Emplace(AudioComponent);
}

void UAudioPlaybackManagerSubsystem::PlayTemporaryAudioOnChannel(FName Channel, USoundBase *Sound,
    float VolumeMultiplier, float PitchMultiplier) {
    auto &ChannelStack = AudioChannels.FindOrAdd(Channel).Data;
    if (!ChannelStack.IsEmpty()) {
        ChannelStack.Top()->SetPaused(true);
    }
    
    auto AudioComponent = UGameplayStatics::SpawnSound2D(this, Sound, VolumeMultiplier, PitchMultiplier, 0, nullptr, true);
    AudioComponent->OnAudioFinishedNative.AddWeakLambda(this, [this, Channel](UAudioComponent* Component) {
        if (auto &FoundStack = AudioChannels.FindOrAdd(Channel).Data; !FoundStack.IsEmpty() && FoundStack.Top() == Component) {
            RemoveTopOfStack(FoundStack, [](UAudioComponent* Element) { Element->SetPaused(false); });
        }
    });
    ChannelStack.Emplace(AudioComponent);
}

void UAudioPlaybackManagerSubsystem::StopCurrentAudioOnChannel(FName Channel) {
    auto &ChannelStack = AudioChannels.FindOrAdd(Channel).Data;
    if (ChannelStack.IsEmpty()) {
        return;
    }

    RemoveTopOfStack(ChannelStack, [](UAudioComponent* Element) { Element->SetPaused(false); });
}