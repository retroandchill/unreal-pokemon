// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Switching/SwitchActionBase.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Events/BattleMessage.h"
#include "Battle/Events/SwitchPokemonPayload.h"
#include "Battle/Tags.h"
#include "Battle/Animations/BattleAnimation.h"
#include "Battle/Animations/BattleAnimationGetter.h"

USwitchActionBase::USwitchActionBase() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::SwitchOut;
}

UE5Coro::GAS::FAbilityCoroutine USwitchActionBase::ExecuteAbility(FGameplayAbilitySpecHandle Handle,
                                                                  const FGameplayAbilityActorInfo *ActorInfo,
                                                                  FGameplayAbilityActivationInfo ActivationInfo,
                                                                  const FGameplayEventData *TriggerEventData) {
    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    auto Payload = CastChecked<const USwitchPokemonPayload>(TriggerEventData->OptionalObject);
    OwningTrainer = Payload->OwningTrainer;
    UserIndex = Payload->UserIndex;
    SwitchTargetIndex = Payload->SwapIndex;

    TScriptInterface<IBattler> Battler = GetCurrentActorInfo()->AvatarActor.Get();
    check(Battler != nullptr)
    SwapTarget = Battler->GetOwningSide()->GetTrainerParty(OwningTrainer)[SwitchTargetIndex];
    auto &AnimationGetter = UBattleAnimationGetter::Get(Battler);
    co_await IBattleAnimation::PlayAnimation(AnimationGetter.GetRecallAnimation(Battler));
    co_await SwapWithTarget(AnimationGetter);
    co_await TriggerOnSendOut();
}

UE5Coro::TCoroutine<> USwitchActionBase::SwapWithTarget(UBattleAnimationGetter& AnimationGetter) {
    auto &ActorInfo = *GetCurrentActorInfo();
    auto Battler = CastChecked<IBattler>(ActorInfo.AvatarActor);
    Battler->HideSprite();
    auto &OwningSide = Battler->GetOwningSide();
    OwningSide->SwapBattlerPositions(OwningTrainer, UserIndex, SwitchTargetIndex);

    auto TargetActor = CastChecked<AActor>(SwapTarget.GetObject());
    TargetActor->SetActorTransform(ActorInfo.AvatarActor->GetActorTransform());
    co_await IBattleAnimation::PlayAnimation(AnimationGetter.GetSendOutAnimation(SwapTarget));
}

UE5Coro::TCoroutine<> USwitchActionBase::TriggerOnSendOut() {
    auto &Battle = SwapTarget->GetOwningSide()->GetOwningBattle();
    co_await Battle->OnBattlersEnteringBattle(Retro::Ranges::Views::Single(SwapTarget));
}