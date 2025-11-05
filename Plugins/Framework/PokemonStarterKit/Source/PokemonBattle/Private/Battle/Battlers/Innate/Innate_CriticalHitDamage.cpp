// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/Innate/Innate_CriticalHitDamage.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Settings/PokemonBattleSettings.h"

UInnate_CriticalHitDamage::UInnate_CriticalHitDamage() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Moves::DamageModificationEvents.UserTag;
}

void UInnate_CriticalHitDamage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo *ActorInfo,
                                                const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    check(TriggerEventData != nullptr)
    auto Payload = CastChecked<UDamageModificationPayload>(TriggerEventData->OptionalObject);
    auto &Data = Payload->GetData();
    if (Data.Target->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit)) {
        Payload->SetFinalDamageMultiplier(Data.FinalDamageMultiplier *
                                          GetDefault<UPokemonBattleSettings>()->CriticalHitMultiplier);
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}