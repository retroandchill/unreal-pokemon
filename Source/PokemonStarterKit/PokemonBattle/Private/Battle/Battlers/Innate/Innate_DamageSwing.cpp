// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/Innate/Innate_DamageSwing.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "Battle/Moves/MoveTags.h"

constexpr int32 MinimumSwing = 85;
constexpr int32 Variance = 100 - MinimumSwing;

UInnate_DamageSwing::UInnate_DamageSwing() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Moves::DamageModificationEvents.UserTag;
}

void UInnate_DamageSwing::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo *ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    check(TriggerEventData != nullptr)
    auto Payload = CastChecked<UDamageModificationPayload>(TriggerEventData->OptionalObject);
    int32 Roll = MinimumSwing + FMath::Rand() % Variance;
    float Multiplier = static_cast<float>(Roll) / 100.f;
    Payload->SetFinalDamageMultiplier(Payload->GetData().FinalDamageMultiplier * Multiplier);
    EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}