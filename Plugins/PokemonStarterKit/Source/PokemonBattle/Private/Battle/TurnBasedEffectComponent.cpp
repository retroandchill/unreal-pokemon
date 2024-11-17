// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/TurnBasedEffectComponent.h"
#include "AbilitySystemComponent.h"


void UTurnBasedEffectComponent::BeginPlay() {
    Super::BeginPlay();
    AbilitySystemComponent = GetOwner()->GetComponentByClass<UAbilitySystemComponent>();
}

void UTurnBasedEffectComponent::AddTurnBasedEffect(FActiveGameplayEffectHandle Handle, int32 TurnDuration) {
    ActiveTurnBasedEffects.Emplace(Handle, FTurnBasedGameplayEffect(this, Handle, TurnDuration));
}

bool UTurnBasedEffectComponent::RemoveTurnBasedEffect(FActiveGameplayEffectHandle Handle) {
    return ActiveTurnBasedEffects.Remove(Handle) > 0;
}