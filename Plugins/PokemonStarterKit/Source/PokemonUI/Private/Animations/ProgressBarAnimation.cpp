// "Unreal Pokémon" created by Retro & Chill.

#include "Animations/ProgressBarAnimation.h"
#include "PokemonUI.h"

using namespace Pokemon::UI;

FBarAnimationData::FBarAnimationData(float StartingPercentage, float EndingPercentage, float AnimationDuration, bool bWrapAround)
    : StartingPercentage(StartingPercentage), EndPercentage(EndingPercentage), AnimationDuration(AnimationDuration), bWrapAround(bWrapAround) {
}

void FProgressBarAnimation::PlayAnimation(float StartPercent, float EndPercent, float Duration, bool bShouldWrap) {
    if (AnimationData.IsSet()) {
        UE_LOG(LogPokemonUI, Warning,
               TEXT("Attempted to animate a progress bar that already had an animation in progress!"))
        return;
    }

    AnimationData.Emplace(StartPercent, EndPercent, Duration, bShouldWrap);
    PercentLastTick = StartPercent;
}

void FProgressBarAnimation::BindActionToPercentDelegate(FSetNewPercent::FDelegate &&Binding) {
    SetNewPercent.Add(MoveTemp(Binding));
}

void FProgressBarAnimation::BindActionToWrapAroundAnimation(FOnAnimationComplete::FDelegate &&Binding) {
    OnBarWrapAround.Add(MoveTemp(Binding));
}

void FProgressBarAnimation::BindActionToCompleteDelegate(FOnAnimationComplete::FDelegate &&Binding) {
    OnAnimationComplete.Add(MoveTemp(Binding));
}

void FProgressBarAnimation::Tick(float DeltaTime) {
    if (!AnimationData.IsSet()) {
        return;
    }

    auto &[StartingPercentage, EndPercentage, AnimationDuration, CurrentTime, bWrapAround] = AnimationData.GetValue();
    CurrentTime += DeltaTime;
    float NewPercent = FMath::Lerp(StartingPercentage, EndPercentage, CurrentTime / AnimationDuration);
    if (bWrapAround && StartingPercentage < EndPercentage) {
        NewPercent = FMath::Fmod(NewPercent, 1.f);
        if (PercentLastTick > NewPercent) {
            OnBarWrapAround.Broadcast();
        }
    }
    
    SetNewPercent.Broadcast(NewPercent);
    PercentLastTick = NewPercent;
    if (CurrentTime >= AnimationDuration) {
        AnimationData.Reset();
        OnAnimationComplete.Broadcast();
    }
}

TStatId FProgressBarAnimation::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHPBar, STATGROUP_Tickables)
}