// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/TargetedEvents.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Algo/ForEach.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Effects/TurnBasedEffectComponent.h"
#include "Ranges/Algorithm/ForEach.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Views/CastType.h"
#include "Ranges/Views/Concat.h"
#include "Ranges/Views/ContainerView.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Views/Join.h"
#include "Ranges/Views/Single.h"

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
    auto SideView = UE::Ranges::Single(Side) |
                    UE::Ranges::CastType<AActor>;
    auto ActiveBattlers = Side->GetBattlers() |
                          UE::Ranges::Filter(&IBattler::IsNotFainted) |
                          UE::Ranges::CastType<AActor>;
    // clang-format on
    return UE::Ranges::Concat(std::move(SideView), std::move(ActiveBattlers));
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
    Battle->GetSides() | UE::Ranges::Map(&UnrollBattleSide) | UE::Ranges::Join |
        UE::Ranges::ForEach(&SendOutEventForActor, Tag, EventData);
}

void Pokemon::Battle::Events::SendOutMoveEvent(const TScriptInterface<IBattler> &User, const UObject *Payload,
                                               const FNativeGameplayTag &EventTag) {
    auto UserActor = UE::Ranges::CastInterfaceChecked<AActor>(User);
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(std::move(TargetData));
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
    EventData.TargetData.Data.Emplace(std::move(TargetData));

    SendOutEventForActor(UserActor, EventTags.GlobalTag, EventData);
    SendOutEventForActor(UserActor, EventTags.UserTag, EventData);
    User->GetAllies() | UE::Ranges::Filter(&IBattler::IsNotFainted) | UE::Ranges::CastType<AActor> |
        UE::Ranges::ForEach([&EventTags, &EventData](AActor *Ally) {
            SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
            SendOutEventForActor(Ally, EventTags.UserAllyTag, EventData);
        });

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    SendOutEventForActor(TargetActor, EventTags.GlobalTag, EventData);
    SendOutEventForActor(TargetActor, EventTags.TargetTag, EventData);
    Target->GetAllies() | UE::Ranges::Filter(&IBattler::IsNotFainted) | UE::Ranges::CastType<AActor> |
        UE::Ranges::ForEach([&EventTags, &EventData](AActor *Ally) {
            SendOutEventForActor(Ally, EventTags.GlobalTag, EventData);
            SendOutEventForActor(Ally, EventTags.TargetAllyTag, EventData);
        });

    auto UserSide = User->GetOwningSide();
    SendOutEventForActor(CastChecked<AActor>(UserSide.GetObject()), EventTags.UserSideTag, EventData);
    SendOutEventForActor(CastChecked<AActor>(Target->GetOwningSide().GetObject()), EventTags.TargetSideTag, EventData);
    SendOutEventForActor(CastChecked<AActor>(UserSide->GetOwningBattle().GetObject()), EventTags.BattlefieldTag,
                         EventData);
}