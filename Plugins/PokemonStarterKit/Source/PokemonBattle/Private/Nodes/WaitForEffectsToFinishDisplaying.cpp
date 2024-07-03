// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/WaitForEffectsToFinishDisplaying.h"
#include "Battle/Effects/AbilityDisplayComponent.h"

using FCallback = FDisplayQueueEmpty::FDelegate;

UWaitForEffectsToFinishDisplaying * UWaitForEffectsToFinishDisplaying::WaitForEffectsToFinishDisplaying(
    UGameplayAbility *Ability) {
    auto Node = NewObject<UWaitForEffectsToFinishDisplaying>();
    Node->GameplayAbility = Ability;
    return Node;
}

void UWaitForEffectsToFinishDisplaying::Activate() {
    auto AbilityDisplayComponent = IAbilityDisplayComponent::FindAbilityDisplayComponent(GameplayAbility);
    check(AbilityDisplayComponent != nullptr)

    if (!AbilityDisplayComponent->IsAbilityDisplaying()) {
        OnDisplayFinished.Broadcast();
        return;
    }

    DelegateHandle = AbilityDisplayComponent->BindToDisplayQueueEmpty(FCallback::CreateUObject(this,
        &UWaitForEffectsToFinishDisplaying::ExecuteOnDisplayFinished, AbilityDisplayComponent));
}

void UWaitForEffectsToFinishDisplaying::ExecuteOnDisplayFinished(TScriptInterface<IAbilityDisplayComponent> AbilityDisplayComponent) const {
    OnDisplayFinished.Broadcast();
    AbilityDisplayComponent->RemoveQueueEmptyBinding(DelegateHandle);
}