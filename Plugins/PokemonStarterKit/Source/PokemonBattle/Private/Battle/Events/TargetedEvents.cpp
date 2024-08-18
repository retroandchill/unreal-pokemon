// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/TargetedEvents.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/CastType.h"
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
    auto SideView = ranges::views::single(Side) | UE::Ranges::CastType<AActor>;
    auto ActiveBattlers = Side->GetBattlers() | ranges::views::filter(&IBattler::IsNotFainted) |
                          UE::Ranges::CastType<AActor>;
    return ranges::views::concat(SideView, ActiveBattlers);
}

static void SendOutEventForActor(AActor *Actor, const FGameplayTag &Tag, FGameplayEventData &EventData) {
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
    SendOutEventForActor(BattleActor, Tag, EventData);
    Battle->GetSides() |
        ranges::views::transform(&UnrollBattleSide) |
        ranges::views::join |
        UE::Ranges::ForEach(&SendOutEventForActor, Tag, EventData);
}

void Pokemon::Battle::Events::SendOutMoveEvent(const TScriptInterface<IBattler> &User, const UObject *Payload,
                                               const FNativeGameplayTag &EventTag) {
    auto UserActor = UE::Ranges::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));
    SendOutEventForActor(UserActor, EventTag, EventData);
}

void Pokemon::Battle::Events::SendOutMoveEvents(const TScriptInterface<IBattler> &User,
                                                const TScriptInterface<IBattler> &Target, const UObject *Payload,
                                                const FTargetedEvent &EventTags) {
    auto UserActor = UE::Ranges::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));

    SendOutEventForActor(UserActor, EventTags.GlobalTag, EventData);
    SendOutEventForActor(UserActor, EventTags.UserTag, EventData);
    User->GetAllies() |
        ranges::views::filter(&IBattler::IsNotFainted) |
        UE::Ranges::CastType<AActor> |
        UE::Ranges::ForEach([&EventTags, &EventData](AActor* Ally) {
            SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
            SendOutEventForActor(Ally, EventTags.UserAllyTag, EventData);
        });

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    SendOutEventForActor(TargetActor, EventTags.GlobalTag, EventData);
    SendOutEventForActor(TargetActor, EventTags.TargetTag, EventData);
    Target->GetAllies() |
        ranges::views::filter(&IBattler::IsNotFainted) |
        UE::Ranges::CastType<AActor> |
            UE::Ranges::ForEach([&EventTags, &EventData](AActor* Ally) {
            SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
            SendOutEventForActor(Ally, EventTags.TargetAllyTag, EventData);
        });

    auto UserSide = User->GetOwningSide();
    SendOutEventForActor(CastChecked<AActor>(UserSide.GetObject()), EventTags.UserSideTag, EventData);
    SendOutEventForActor(CastChecked<AActor>(Target->GetOwningSide().GetObject()), EventTags.TargetSideTag, EventData);
    SendOutEventForActor(CastChecked<AActor>(UserSide->GetOwningBattle().GetObject()), EventTags.BattlefieldTag,
                         EventData);
}