// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/TargetedEvents.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

const FNativeGameplayTag & FTargetedEvent::GetTagForScope(ETargetedEventScope Scope) {
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

static AActor* ConvertToActor(const TScriptInterface<IBattler>& Battler) {
    return CastChecked<AActor>(Battler.GetObject());
}

static bool AllyNotFainted(const TScriptInterface<IBattler>& Battler) {
    return !Battler->IsFainted();
}

static void SendOutEventForActor(const FGameplayTag &Tag, FGameplayEventData &EventData,
                                             AActor* Actor) {
    EventData.Target = Actor;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, Tag, EventData);
}

void Pokemon::Battle::Events::SendOutMoveEvent(const TScriptInterface<IBattler> &User, const UObject *Payload,
    const FNativeGameplayTag &EventTag) {
    auto UserActor = CastChecked<AActor>(User.GetObject());
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));
    SendOutEventForActor(EventTag, EventData, UserActor);
}

void Pokemon::Battle::Events::SendOutMoveEvents(const TScriptInterface<IBattler> &User,
                                                const TScriptInterface<IBattler> &Target, const UObject* Payload, const FTargetedEvent &EventTags) {
    using namespace ranges::views;
    auto UserActor = CastChecked<AActor>(User.GetObject());
    FGameplayEventData EventData;
    EventData.OptionalObject = Payload;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));
    
    SendOutEventForActor(EventTags.GlobalTag, EventData, UserActor);
    SendOutEventForActor(EventTags.UserTag, EventData, UserActor);
    for (AActor *Ally : User->GetAllies() | filter(&AllyNotFainted) | transform(&ConvertToActor)) {
        SendOutEventForActor(EventTags.GlobalTag, EventData, Ally);
        SendOutEventForActor(EventTags.UserAllyTag, EventData, Ally);
    }

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    SendOutEventForActor(EventTags.GlobalTag, EventData, TargetActor);
    SendOutEventForActor(EventTags.TargetTag, EventData, TargetActor);
    for (AActor *Ally : Target->GetAllies() | filter(&AllyNotFainted) | transform(&ConvertToActor)) {
        SendOutEventForActor(EventTags.GlobalTag, EventData, Ally);
        SendOutEventForActor(EventTags.TargetAllyTag, EventData, Ally);
    }

    auto UserSide = User->GetOwningSide();
    SendOutEventForActor(EventTags.UserSideTag, EventData, CastChecked<AActor>(UserSide.GetObject()));
    SendOutEventForActor(EventTags.TargetSideTag, EventData, CastChecked<AActor>(Target->GetOwningSide().GetObject()));
    SendOutEventForActor(EventTags.BattlefieldTag, EventData, CastChecked<AActor>(UserSide->GetOwningBattle().GetObject()));
}