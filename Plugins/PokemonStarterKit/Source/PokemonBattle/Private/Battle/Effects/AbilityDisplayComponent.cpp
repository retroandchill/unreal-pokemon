// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/AbilityDisplayComponent.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"


// Add default functionality here for any IAbilityDisplayComponent functions that are not pure virtual.
TScriptInterface<IAbilityDisplayComponent> IAbilityDisplayComponent::FindAbilityDisplayComponent(
    const UGameplayAbility *Ability) {
    auto OwningActor = Ability->GetOwningActorFromActorInfo();
    if (auto Battle = Cast<IBattle>(OwningActor); Battle != nullptr) {
        return Battle->GetAbilityDisplayComponent();
    }

    if (auto BattleSide = Cast<IBattleSide>(OwningActor); BattleSide != nullptr) {
        return BattleSide->GetOwningBattle()->GetAbilityDisplayComponent();
    }

    if (auto Battler = Cast<IBattler>(OwningActor); Battler != nullptr) {
        return Battler->GetOwningSide()->GetOwningBattle()->GetAbilityDisplayComponent();
    }

    return nullptr;
}