// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/Innate/Innate_MultiTargetDamageSplit.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "Battle/Moves/MoveTags.h"

constexpr float MultiTargetMultiplier = 0.75f;

UInnate_MultiTargetDamageSplit::UInnate_MultiTargetDamageSplit()
{
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Moves::DamageModificationEvents.UserTag;
}

void UInnate_MultiTargetDamageSplit::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo *ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     const FGameplayEventData *TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    check(TriggerEventData != nullptr)
    auto Payload = CastChecked<UDamageModificationPayload>(TriggerEventData->OptionalObject);
    auto &Data = Payload->GetData();
    if (Data.TargetCount > 1)
    {
        Payload->SetFinalDamageMultiplier(Data.FinalDamageMultiplier * MultiTargetMultiplier);
    }
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}