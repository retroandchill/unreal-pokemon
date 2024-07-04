// "Unreal Pokémon" created by Retro & Chill.


#include "Animations/ProgressBarAnimation.h"
#include "PokemonUI.h"

using namespace Pokemon::UI;


FBarAnimationData::FBarAnimationData(float StartingPercentage, float EndingPercentage, float AnimationDuration) : StartingPercentage(StartingPercentage), EndPercentage(EndingPercentage), AnimationDuration(AnimationDuration) {
}

void FProgressBarAnimation::PlayAnimation(float StartPercent, float EndPercent, float Duration) {
    if (AnimationData.IsSet()) {
        UE_LOG(LogPokemonUI, Warning, TEXT("Attempted to animate a progress bar that already had an animation in progress!"))
        return;
    }

    if (FMath::IsNearlyEqual(StartPercent, EndPercent)) {
        OnAnimationComplete.Broadcast();
    } else {
        AnimationData.Emplace(StartPercent, EndPercent, Duration);
    }
}

void FProgressBarAnimation::BindActionToPercentDelegate(FSetNewPercent::FDelegate &&Binding) {
    SetNewPercent.Add(MoveTemp(Binding));
}

void FProgressBarAnimation::BindActionToCompleteDelegate(FOnAnimationComplete::FDelegate &&Binding) {
    OnAnimationComplete.Add(MoveTemp(Binding));
}

void FProgressBarAnimation::Tick(float DeltaTime) {
    if (!AnimationData.IsSet()) {
        return;
    }

    auto &[StartingPercentage, EndPercentage, AnimationDuration, CurrentTime] = AnimationData.GetValue();
    CurrentTime += DeltaTime;
    SetNewPercent.Broadcast(FMath::Lerp(StartingPercentage, EndPercentage, CurrentTime / AnimationDuration));
    if (CurrentTime >= AnimationDuration) {
        AnimationData.Reset();
        OnAnimationComplete.Broadcast();
    }
}

TStatId FProgressBarAnimation::GetStatId() const {
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHPBar, STATGROUP_Tickables)
}