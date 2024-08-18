// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/TargetedEvents.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/ContainerView.h"
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/view/concat.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

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
    auto SideView = ranges::views::single(Side) | ranges::views::transform(&UE::Ranges::CastInterfaceChecked<AActor>);
    auto ActiveBattlers = Side->GetBattlers() | ranges::views::filter(&IBattler::IsNotFainted) |
                          ranges::views::transform(&UE::Ranges::CastInterfaceChecked<AActor>);
    return ranges::views::concat(SideView, ActiveBattlers);
}

static void SendOutEventForActor(const FGameplayTag &Tag, FGameplayEventData &EventData, AActor *Actor) {
    EventData.Target = Actor;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, Tag, EventData);
}

void Pokemon::Battle::Events::SendOutBattleEvent(const TScriptInterface<IBattle> &Battle, const UObject *Payload,
                                                 const FGameplayTag &Tag) {
    auto BattleActor = UE::Ranges::CastInterfaceChecked<AActor>(Battle);
    FGameplayEventData EventData;
    EventData.Instigator = BattleActor;
    EventData.OptionalObject = Payload;
    EventData.EventTag = Tag;
    SendOutEventForActor(Tag, EventData, BattleActor);
    auto Children = Battle->GetSides() | ranges::views::transform(&UnrollBattleSide) | ranges::views::join;
    ranges::for_each(Children, [&Tag, &EventData](AActor *Actor) { SendOutEventForActor(Tag, EventData, Actor); });
}

void Pokemon::Battle::Events::SendOutMoveEvent(const TScriptInterface<IBattler> &User, const UObject *Payload,
                                               const FNativeGameplayTag &EventTag) {
    auto UserActor = UE::Ranges::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));
    SendOutEventForActor(EventTag, EventData, UserActor);
}

void Pokemon::Battle::Events::SendOutMoveEvents(const TScriptInterface<IBattler> &User,
                                                const TScriptInterface<IBattler> &Target, const UObject *Payload,
                                                const FTargetedEvent &EventTags) {
    using namespace ranges::views;
    auto UserActor = UE::Ranges::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));

    SendOutEventForActor(EventTags.GlobalTag, EventData, UserActor);
    SendOutEventForActor(EventTags.UserTag, EventData, UserActor);
    for (AActor *Ally : User->GetAllies() | filter(&IBattler::IsNotFainted) | transform(&UE::Ranges::CastInterfaceChecked<AActor>)) {
        SendOutEventForActor(EventTags.GlobalTag, EventData, Ally);
        SendOutEventForActor(EventTags.UserAllyTag, EventData, Ally);
    }

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    SendOutEventForActor(EventTags.GlobalTag, EventData, TargetActor);
    SendOutEventForActor(EventTags.TargetTag, EventData, TargetActor);
    for (AActor *Ally : Target->GetAllies() | filter(&IBattler::IsNotFainted) | transform(&UE::Ranges::CastInterfaceChecked<AActor>)) {
        SendOutEventForActor(EventTags.GlobalTag, EventData, Ally);
        SendOutEventForActor(EventTags.TargetAllyTag, EventData, Ally);
    }

    auto UserSide = User->GetOwningSide();
    SendOutEventForActor(EventTags.UserSideTag, EventData, CastChecked<AActor>(UserSide.GetObject()));
    SendOutEventForActor(EventTags.TargetSideTag, EventData, CastChecked<AActor>(Target->GetOwningSide().GetObject()));
    SendOutEventForActor(EventTags.BattlefieldTag, EventData,
                         CastChecked<AActor>(UserSide->GetOwningBattle().GetObject()));
}