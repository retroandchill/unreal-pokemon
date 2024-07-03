// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/QueueDisplayAndWaitForTurn.h"
#include "AbilitySystemComponent.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Effects/AbilityDisplayComponent.h"

UQueueDisplayAndWaitForTurn * UQueueDisplayAndWaitForTurn::QueueDisplayAndWaitForTurn(UGameplayAbility *Ability) {
        auto Node = NewObject<UQueueDisplayAndWaitForTurn>();
        Node->GameplayAbility = Ability;
        return Node;
}

void UQueueDisplayAndWaitForTurn::Activate() {
    auto AbilityDisplayComponent = FindAbilityDisplayComponent();
    check(AbilityDisplayComponent != nullptr)

    auto AbilitySpecHandle = GameplayAbility->GetCurrentAbilitySpecHandle();
    auto OwningAbilitySystemComponent = GameplayAbility->GetAbilitySystemComponentFromActorInfo();
    AbilityDisplayComponent->QueueAbilityToDisplay(AbilitySpecHandle, OwningAbilitySystemComponent,
        FOnGameplayAbilityDisplay::CreateUObject(this, &UQueueDisplayAndWaitForTurn::ExecuteOnTurn));
}

TScriptInterface<IAbilityDisplayComponent> UQueueDisplayAndWaitForTurn::FindAbilityDisplayComponent() const {
    auto OwningActor = GameplayAbility->GetOwningActorFromActorInfo();
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

void UQueueDisplayAndWaitForTurn::ExecuteOnTurn() const {
    OnTurnToDisplay.Broadcast();
}