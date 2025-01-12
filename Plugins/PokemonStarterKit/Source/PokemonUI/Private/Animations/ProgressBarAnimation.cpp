// "Unreal Pokémon" created by Retro & Chill.

#include "Animations/ProgressBarAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "PokemonUI.h"

namespace Pokemon::UI {
    FBarAnimationData::FBarAnimationData(float StartingPercentage, float EndingPercentage, float AnimationDuration,
                                         bool bWrapAround)
        : StartingPercentage(StartingPercentage), EndPercentage(EndingPercentage), AnimationDuration(AnimationDuration),
          bWrapAround(bWrapAround) {
    }

    UE5Coro::TCoroutine<> ProgressBarAnimation(UE5Coro::TLatentContext<const UObject> Context, float StartPercent,
                                               float EndPercent, float Duration, FSetNewPercent OnUpdate,
                                               bool bShouldWrap, FSimpleDelegate OnWrapAround) {
        if (Duration == 0.f) {
            OnUpdate.ExecuteIfBound(EndPercent);
            co_return;
        }

        float PercentLastTick = StartPercent;
        UE5Coro::Latent::Timeline(Context.Target, 0, 1, Duration, [&](double Progress) {
            float NewPercent = FMath::Lerp(StartPercent, EndPercent, Progress);
            if (bShouldWrap && StartPercent < EndPercent) {
                NewPercent = FMath::Fmod(NewPercent, 1.f);
                if (PercentLastTick > NewPercent) {
                    OnWrapAround.ExecuteIfBound();
                }
            }

            OnUpdate.ExecuteIfBound(NewPercent);
        });
    }
} // namespace Pokemon::UI