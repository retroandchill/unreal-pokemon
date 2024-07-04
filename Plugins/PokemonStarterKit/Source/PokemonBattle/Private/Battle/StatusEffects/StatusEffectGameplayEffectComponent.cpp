// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/StatusEffectGameplayEffectComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/StatusEffects/StatusEffectTags.h"

bool UStatusEffectGameplayEffectComponent::CanGameplayEffectApply(
    const FActiveGameplayEffectsContainer &ActiveGEContainer, const FGameplayEffectSpec &GESpec) const {
    TScriptInterface<IBattler> Battler = ActiveGEContainer.Owner->GetOwnerActor();
    check(Battler != nullptr)
    return !Battler->GetStatusEffect().IsSet();
}

bool UStatusEffectGameplayEffectComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &GEContainer,
    FActiveGameplayEffect &ActiveGE) const {
    TScriptInterface<IBattler> Battler = GEContainer.Owner->GetOwnerActor();
    check(Battler != nullptr)
    Battler->InflictStatusEffect(StatusEffectID, ActiveGE.Handle);
    auto EventSet = GEContainer.Owner->GetActiveEffectEventSet(ActiveGE.Handle);
    static auto& Lookup = Pokemon::Battle::StatusEffects::FLookup::GetInstance();
    EventSet->OnEffectRemoved.AddUObject(this, &UStatusEffectGameplayEffectComponent::OnGameplayEffectRemoved, Battler);
    Battler->GetAbilityComponent()->AddLooseGameplayTag(Lookup.GetTag(StatusEffectID));
    return Super::OnActiveGameplayEffectAdded(GEContainer, ActiveGE);
}

void UStatusEffectGameplayEffectComponent::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo&, TScriptInterface<IBattler> Battler) const {
    check(Battler->GetStatusEffect().IsSet())
    Battler->CureStatusEffect();
    static auto& Lookup = Pokemon::Battle::StatusEffects::FLookup::GetInstance();
    Battler->GetAbilityComponent()->RemoveLooseGameplayTag(Lookup.GetTag(StatusEffectID));
}