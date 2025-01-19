// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Switching/SwitchAction.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Events/SwitchPokemonPayload.h"
#include "Battle/Tags.h"
#include "Battle/Animations/BattleAnimation.h"
#include "Battle/Animations/BattleAnimationGetter.h"
#include "Battle/Settings/BattleMessageSettings.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

USwitchAction::USwitchAction() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::SwitchOut;
}

UE5Coro::GAS::FAbilityCoroutine USwitchAction::ExecuteAbility(FGameplayAbilitySpecHandle Handle,
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
    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Battler);
    if (!Battler->IsFainted()) {
        co_await Dispatcher.DisplayMessage(Battler->GetRecallMessage());
    }
    co_await IBattleAnimation::PlayAnimation(AnimationGetter.GetRecallAnimation(Battler));
    co_await SwapWithTarget(Battler, AnimationGetter, Dispatcher);
    co_await TriggerOnSendOut();
}

UE5Coro::TCoroutine<> USwitchAction::SwapWithTarget(const TScriptInterface<IBattler>& SwappingFrom, UBattleAnimationGetter& AnimationGetter, IPokemonCoroutineDispatcher& Dispatcher) {
    auto &ActorInfo = *GetCurrentActorInfo();
    auto Battler = CastChecked<IBattler>(ActorInfo.AvatarActor);
    Battler->HideSprite();
    auto &OwningSide = Battler->GetOwningSide();
    OwningSide->SwapBattlerPositions(OwningTrainer, UserIndex, SwitchTargetIndex);

    auto TargetActor = CastChecked<AActor>(SwapTarget.GetObject());
    TargetActor->SetActorTransform(ActorInfo.AvatarActor->GetActorTransform());

    auto Messages = GetDefault<UBattleMessageSettings>();
    auto &SendOutFormat = SwapTarget->IsOwnedByPlayer() ? Messages->PlayerSendOutMessage : Messages->OpponentSendOutMessage;
    co_await Dispatcher.DisplayMessage(FText::FormatNamed(SendOutFormat, "Pkmn", SwapTarget->GetNickname(), "Trainer", OwningTrainer->GetFullTrainerName()));
    co_await IBattleAnimation::PlayAnimation(AnimationGetter.GetSendOutAnimation(SwapTarget, SwappingFrom));
}

UE5Coro::TCoroutine<> USwitchAction::TriggerOnSendOut() {
    auto &Battle = SwapTarget->GetOwningSide()->GetOwningBattle();
    co_await Battle->OnBattlersEnteringBattle(Retro::Ranges::Views::Single(SwapTarget));
}