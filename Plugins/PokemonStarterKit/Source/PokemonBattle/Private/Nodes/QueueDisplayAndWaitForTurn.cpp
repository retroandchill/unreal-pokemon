// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/QueueDisplayAndWaitForTurn.h"
#include "AbilitySystemComponent.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Effects/AbilityDisplayComponent.h"

UQueueDisplayAndWaitForTurn * UQueueDisplayAndWaitForTurn::QueueDisplayAndWaitForTurn(
    UAbilitySystemComponent *OwningAbilitySystemComponent, FGameplayAbilitySpecHandle AbilitySpecHandle) {
    auto Node = NewObject<UQueueDisplayAndWaitForTurn>();
    Node->AbilitySpecHandle = AbilitySpecHandle;
    Node->OwningAbilitySystemComponent = OwningAbilitySystemComponent;
    return Node;
}

void UQueueDisplayAndWaitForTurn::Activate() {
    auto AbilityDisplayComponent = FindAbilityDisplayComponent();
    check(AbilityDisplayComponent != nullptr)

    AbilityDisplayComponent->QueueAbilityToDisplay(AbilitySpecHandle, OwningAbilitySystemComponent,
        FOnGameplayAbilityDisplay::CreateUObject(this, &UQueueDisplayAndWaitForTurn::ExecuteOnTurn));
}

TScriptInterface<IAbilityDisplayComponent> UQueueDisplayAndWaitForTurn::FindAbilityDisplayComponent() const {
    auto OwningActor = OwningAbilitySystemComponent->GetOwnerActor();
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