// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/TurnBasedGameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Battle/Effects/TurnBasedGameplayEffectComponent.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/IsSet.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/Of.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Optional/OrElse.h"
#include "Ranges/Utilities/Comparison.h"
#include "Ranges/Views/FilterValid.h"
#include "Ranges/Views/Join.h"
#include "Ranges/Views/Single.h"

FTurnBasedGameplayEffect::FTurnBasedGameplayEffect(UTurnBasedEffectComponent *OwningComponent,
                                                   FActiveGameplayEffectHandle EffectHandle,
                                                   int32 TurnDuration) : OwningComponent(OwningComponent),
                                                                         EffectHandle(EffectHandle),
                                                                         GameplayEffect(
                                                                             OwningComponent->
                                                                             GetAbilitySystemComponent()->
                                                                             GetGameplayEffectCDO(EffectHandle)),
                                                                         TurnsRemaining(
                                                                             TurnDuration > 0
                                                                                 ? TOptional(TurnDuration)
                                                                                 : TOptional<int32>()) {

}

bool FTurnBasedGameplayEffect::HasTrigger(ETurnDurationTrigger Trigger) const {
    // clang-format off
    return UE::Optionals::OfNullable(GameplayEffect) |
           UE::Optionals::Map(&UGameplayEffect::FindComponent<UTurnBasedGameplayEffectComponent>) |
           UE::Optionals::Map(&UTurnBasedGameplayEffectComponent::GetTurnDurationTrigger) |
           UE::Optionals::Filter(UE::Ranges::Equals, Trigger) |
           UE::Optionals::IsSet;
    // clang-format on
}

bool FTurnBasedGameplayEffect::IncrementTurnCount(const FRunningMessageSet &RunningMessages) {
    TurnsActive++;
    // clang-format
    return TurnsRemaining |
           UE::Optionals::Filter(UE::Ranges::GreaterThan, TurnsActive) |
           UE::Optionals::Map([this, &RunningMessages](auto) { return RemoveEffect(RunningMessages); }) |
           UE::Optionals::OrElse(false);
    // clang-format off
}

bool FTurnBasedGameplayEffect::RemoveEffect(const FRunningMessageSet& Messages, int32 StacksToRemove) {
    // clang-format
    auto OwnedComponent = UE::Optionals::OfNullable(OwningComponent.Get());
    auto MessageHandle = OwnedComponent |
        UE::Optionals::Map(&UTurnBasedEffectComponent::SetRunningMessages, Messages);
    return OwnedComponent |
                    UE::Optionals::Map(&UTurnBasedEffectComponent::GetAbilitySystemComponent) |
                    UE::Optionals::Map(&UAbilitySystemComponent::RemoveActiveGameplayEffect, EffectHandle, StacksToRemove) |
                    UE::Optionals::OrElse(false);
    // clang-format off
    
}