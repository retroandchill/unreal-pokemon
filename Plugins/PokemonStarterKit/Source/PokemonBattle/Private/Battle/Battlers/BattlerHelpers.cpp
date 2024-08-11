// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerHelpers.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"

float UBattlerHelpers::GetBattlerStat(const FGameplayAbilityActorInfo &ActorInfo, FGameplayAttribute Attribute,
                                      bool &bFound) {
    return ActorInfo.AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
}

EStatusEffectStatus UBattlerHelpers::GetStatusEffect(const TScriptInterface<IBattler> &Battler,
    FStatusEffectInfo &StatusEffect) {
    auto StatusEffectInfo = Battler->GetStatusEffect();
    if (!StatusEffectInfo.IsSet()) {
        return EStatusEffectStatus::NoStatusEffect;
    }
    
    StatusEffect = *StatusEffectInfo;
    return EStatusEffectStatus::HasStatusEffect;
}