// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/TargetedEvents.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PokemonBattleModule.h"
#include "Algo/ForEach.h"
#include "Battle/AsyncAbilityComponent.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Casting/UClassCasts.h"
#include "RetroLib/Functional/Delegates.h"
#include "RetroLib/Optionals/PtrOrNull.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

const FNativeGameplayTag &FTargetedEvent::GetTagForScope(ETargetedEventScope Scope) {
    switch (Scope) {
    case ETargetedEventScope::Global:
        return GlobalTag;
    case ETargetedEventScope::User:
        return UserTag;
    case ETargetedEventScope::UserAlly:
        return TargetTag;
    case ETargetedEventScope::UserSide:
        return UserSideTag;
    case ETargetedEventScope::Target:
        return TargetTag;
    case ETargetedEventScope::TargetAlly:
        return TargetAllyTag;
    case ETargetedEventScope::TargetSide:
        return TargetSideTag;
    default: // Battlefield
        return BattlefieldTag;
    }
}

static auto UnrollBattleSide(const TScriptInterface<IBattleSide> &Side) {
    // clang-format off
    auto SideView = Retro::Ranges::Views::Single(Side) |
                    Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>);
    auto ActiveBattlers = Side->GetBattlers() |
                          Retro::Ranges::Views::Filter(&IBattler::IsNotFainted) |
                          Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>);
    // clang-format on
    return Retro::Ranges::Views::Concat(std::move(SideView), std::move(ActiveBattlers));
}

UE5Coro::TCoroutine<> Pokemon::Battle::Events::SendOutActivationEvent(UAsyncAbilityComponent* AbilityComponent,
                                                                      FGameplayAbilitySpecHandle Handle,
                                                                      FGameplayTag Tag, const FGameplayEventData& EventData,
                                                                      FForceLatentCoroutine) {
    co_await AbilityComponent->TriggerAbilityFromGameplayEventAsync(Handle, AbilityComponent->AbilityActorInfo.Get(), Tag, &EventData, *AbilityComponent);
}

UE5Coro::TCoroutine<> Pokemon::Battle::Events::SendOutBattleEvent(const TScriptInterface<IBattle> &Battle,
                                                                  const UObject *Payload,
                                                                  const FGameplayTag &Tag, FForceLatentCoroutine) {
    auto BattleActor = Retro::CastInterfaceChecked<AActor>(Battle);
    FGameplayEventData EventData;
    EventData.Instigator = BattleActor;
    EventData.OptionalObject = Payload;
    EventData.EventTag = Tag;
    co_await SendOutEventForActor(BattleActor, Tag, EventData);
    // clang-format off
    for (AActor *Actor : Battle->GetSides() | Retro::Ranges::Views::Transform(&UnrollBattleSide) | Retro::Ranges::Views::Join) {
        co_await SendOutEventForActor(Actor, Tag, EventData);
    }
    // clang-format on
}

UE5Coro::TCoroutine<> Pokemon::Battle::Events::SendOutMoveEvent(const TScriptInterface<IBattler> &User,
                                                                const UObject *Payload,
                                                                const FNativeGameplayTag &EventTag) {
    auto UserActor = Retro::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(std::move(TargetData));
    co_await SendOutEventForActor(UserActor, EventTag, EventData);
}

UE5Coro::TCoroutine<> Pokemon::Battle::Events::SendOutMoveEvents(const TScriptInterface<IBattler> &User,
                                                                 const TScriptInterface<IBattler> &Target,
                                                                 const UObject *Payload,
                                                                 const FTargetedEvent &EventTags) {
    auto UserActor = Retro::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(std::move(TargetData));

    co_await SendOutEventForActor(UserActor, EventTags.GlobalTag, EventData);
    co_await SendOutEventForActor(UserActor, EventTags.UserTag, EventData);
    for (auto Ally : User->GetAllies() | Retro::Ranges::Views::Filter(&IBattler::IsNotFainted) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>)) {
        co_await SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
        co_await SendOutEventForActor(Ally, EventTags.UserAllyTag, EventData);
    }

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    co_await SendOutEventForActor(TargetActor, EventTags.GlobalTag, EventData);
    co_await SendOutEventForActor(TargetActor, EventTags.TargetTag, EventData);
    for (auto Ally : Target->GetAllies() | Retro::Ranges::Views::Filter(&IBattler::IsNotFainted) |
        Retro::Ranges::Views::Transform(Retro::DynamicCastChecked<AActor>)) {
            co_await SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
            co_await SendOutEventForActor(Ally, EventTags.TargetAllyTag, EventData);
    }

    auto UserSide = User->GetOwningSide();
    co_await SendOutEventForActor(CastChecked<AActor>(UserSide.GetObject()), EventTags.UserSideTag, EventData);
    co_await SendOutEventForActor(CastChecked<AActor>(Target->GetOwningSide().GetObject()), EventTags.TargetSideTag, EventData);
    co_await SendOutEventForActor(CastChecked<AActor>(UserSide->GetOwningBattle().GetObject()), EventTags.BattlefieldTag,
                         EventData);
}

UE5Coro::TCoroutine<int32> Pokemon::Battle::Events::SendOutEventForActor(AActor *Actor, const FGameplayTag &Tag,
                                                                         FGameplayEventData &EventData,
                                                                         FForceLatentCoroutine) {
    EventData.Target = Actor;
    if (!IsValid(Actor)) {
        co_return 0;
    }
    
    auto AbilitySystemComponent = Actor->GetComponentByClass<UAsyncAbilityComponent>();
    if (!IsValid(AbilitySystemComponent)) {
        UE_LOG(LogBattle, Error,
               TEXT(
                   "Pokemon::Battle::Events::SendOutEventForActor: Invalid ability system component retrieved from Actor %s. EventTag was %s"
               ), *Actor->GetName(), *Tag.ToString());

        co_return 0;
    }
        
    co_return co_await AbilitySystemComponent->HandleGameplayEventAsync(Tag, &EventData);
}