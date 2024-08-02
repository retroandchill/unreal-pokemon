// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Switching/SwitchActionBase.h"
#include "Battle/BattleSide.h"
#include "Battle/Tags.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/SwitchPokemonPayload.h"

USwitchActionBase::USwitchActionBase() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::SwitchOut;
}

void USwitchActionBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    auto Payload = CastChecked<const USwitchPokemonPayload>(TriggerEventData->OptionalObject);
    OwningTrainer = Payload->OwningTrainer;
    UserIndex = Payload->UserIndex;
    SwitchTargetIndex = Payload->SwapIndex;
    
    auto Battler = CastChecked<IBattler>(GetCurrentActorInfo()->AvatarActor);
    SwapTarget = Battler->GetOwningSide()->GetTrainerParty(OwningTrainer)[SwitchTargetIndex];
}

void USwitchActionBase::SwapWithTarget() {
    auto &ActorInfo = *GetCurrentActorInfo();
    auto Battler = CastChecked<IBattler>(ActorInfo.AvatarActor);
    Battler->HideSprite();
    Battler->GetOwningSide()->SwapBattlerPositions(OwningTrainer, UserIndex, SwitchTargetIndex);

    auto TargetActor = CastChecked<AActor>(SwapTarget.GetObject());
    TargetActor->SetActorTransform(ActorInfo.AvatarActor->GetActorTransform());
    PlaySendOutAnimation(SwapTarget);
}