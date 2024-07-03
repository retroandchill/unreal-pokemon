// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/StatusEffectGameplayEffectComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Battle/Battlers/Battler.h"

bool UStatusEffectGameplayEffectComponent::CanGameplayEffectApply(
    const FActiveGameplayEffectsContainer &ActiveGEContainer, const FGameplayEffectSpec &GESpec) const {
    TScriptInterface<IBattler> Battler = ActiveGEContainer.Owner->GetOwnerActor();
    check(Battler != nullptr)
    return !Battler->GetStatusEffect().IsSet() && Super::CanGameplayEffectApply(ActiveGEContainer, GESpec);
}

bool UStatusEffectGameplayEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &GEContainer,
    FActiveGameplayEffect &ActiveGE) const {
    TScriptInterface<IBattler> Battler = GEContainer.Owner->GetOwnerActor();
    check(Battler != nullptr)
    Battler->InflictStatusEffect(StatusEffectID, ActiveGE.Handle);
    auto EventSet = GEContainer.Owner->GetActiveEffectEventSet(ActiveGE.Handle);
    EventSet->OnEffectRemoved.AddStatic(&UStatusEffectGameplayEffectComponent::OnGameplayEffectRemoved, Battler);
    return Super::OnActiveGameplayEffectAdded(GEContainer, ActiveGE);
}

void UStatusEffectGameplayEffectComponent::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo&, TScriptInterface<IBattler> Battler) {
    check(Battler->GetStatusEffect().IsSet())
    Battler->CureStatusEffect();
}