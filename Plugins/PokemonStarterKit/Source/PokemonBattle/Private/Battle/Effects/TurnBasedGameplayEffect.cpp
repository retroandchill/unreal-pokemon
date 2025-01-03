// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Effects/TurnBasedGameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Battle/Effects/TurnBasedGameplayEffectComponent.h"
#include "RetroLib/Optionals/Filter.h"
#include "RetroLib/Optionals/IsSet.h"
#include "RetroLib/Optionals/OrElseValue.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Utils/Operators.h"

FTurnBasedGameplayEffect::FTurnBasedGameplayEffect(UTurnBasedEffectComponent *OwningComponent,
                                                   FActiveGameplayEffectHandle EffectHandle, int32 TurnDuration)
    : OwningComponent(OwningComponent), EffectHandle(EffectHandle),
      GameplayEffect(OwningComponent->GetAbilitySystemComponent()->GetGameplayEffectCDO(EffectHandle)),
      TurnsRemaining(TurnDuration > 0 ? TOptional(TurnDuration) : TOptional<int32>()) {
}

bool FTurnBasedGameplayEffect::HasTrigger(ETurnDurationTrigger Trigger) const {
    // clang-format off
    return Retro::Optionals::OfNullable(GameplayEffect) |
           Retro::Optionals::Transform(&UGameplayEffect::FindComponent<UTurnBasedGameplayEffectComponent>) |
           Retro::Optionals::Transform<&UTurnBasedGameplayEffectComponent::GetTurnDurationTrigger>() |
           Retro::Optionals::Filter(Retro::Equals, Trigger) |
           Retro::Optionals::IsSet;
    // clang-format on
}

bool FTurnBasedGameplayEffect::IncrementTurnCount() {
    TurnsActive++;
    // clang-format off
    return TurnsRemaining |
           Retro::Optionals::Filter(Retro::GreaterThan, TurnsActive) |
           Retro::Optionals::Transform([this](auto) { return RemoveEffect(); }) | 
           Retro::Optionals::OrElseValue(false);
    // clang-format on
}

bool FTurnBasedGameplayEffect::RemoveEffect(int32 StacksToRemove) {
    // clang-format off
    return Retro::Optionals::OfNullable(OwningComponent.Get()) |
                    Retro::Optionals::Transform<&UTurnBasedEffectComponent::GetAbilitySystemComponent>() |
                    Retro::Optionals::Transform<&UAbilitySystemComponent::RemoveActiveGameplayEffect>(EffectHandle, StacksToRemove) |
                    Retro::Optionals::OrElseValue(false);
    // clang-format on
}