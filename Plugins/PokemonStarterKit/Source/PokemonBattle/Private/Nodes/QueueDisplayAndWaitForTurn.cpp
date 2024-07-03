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
    auto AbilityDisplayComponent = IAbilityDisplayComponent::FindAbilityDisplayComponent(GameplayAbility);
    check(AbilityDisplayComponent != nullptr)

    auto AbilitySpecHandle = GameplayAbility->GetCurrentAbilitySpecHandle();
    auto OwningAbilitySystemComponent = GameplayAbility->GetAbilitySystemComponentFromActorInfo();
    AbilityDisplayComponent->QueueAbilityToDisplay(AbilitySpecHandle, OwningAbilitySystemComponent,
        FOnGameplayAbilityDisplay::CreateUObject(this, &UQueueDisplayAndWaitForTurn::ExecuteOnTurn));
}

void UQueueDisplayAndWaitForTurn::ExecuteOnTurn() const {
    OnTurnToDisplay.Broadcast();
}