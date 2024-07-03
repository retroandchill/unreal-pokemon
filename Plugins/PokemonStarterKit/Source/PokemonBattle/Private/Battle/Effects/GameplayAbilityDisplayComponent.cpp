// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Effects/GameplayAbilityDisplayComponent.h"
#include "AbilitySystemComponent.h"


FAbilityDisplay::FAbilityDisplay(FGameplayAbilitySpecHandle AbilitySpecHandle,
    UAbilitySystemComponent *AbilitySystemComponent, FOnGameplayAbilityDisplay &&ContinueDelegate) : Ability(AbilitySpecHandle), AbilitySystemComponent(AbilitySystemComponent), ContinueDelegate(MoveTemp(ContinueDelegate)) {
}

UGameplayAbilityDisplayComponent::UGameplayAbilityDisplayComponent() {
    PrimaryComponentTick.bCanEverTick = true;
}

void UGameplayAbilityDisplayComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                     FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    ProcessAbilitiesQueue();
}

void UGameplayAbilityDisplayComponent::QueueAbilityToDisplay(FGameplayAbilitySpecHandle Handle,
                                                             UAbilitySystemComponent* AbilitySystemComponent, FOnGameplayAbilityDisplay &&ContinueDelegate) {
    AbilitiesToDisplay.Enqueue(FAbilityDisplay(Handle, AbilitySystemComponent, MoveTemp(ContinueDelegate)));
}

bool UGameplayAbilityDisplayComponent::IsAbilityDisplaying() const {
    return !AbilitiesToDisplay.IsEmpty();
}

FDelegateHandle UGameplayAbilityDisplayComponent::BindToDisplayQueueEmpty(FDisplayQueueEmpty::FDelegate&& Delegate) {
    return OnQueueEmpty.Add(MoveTemp(Delegate));
}

void UGameplayAbilityDisplayComponent::RemoveQueueEmptyBinding(FDelegateHandle DelegateHandle) {
    OnQueueEmpty.Remove(DelegateHandle);
}

void UGameplayAbilityDisplayComponent::ProcessAbilitiesQueue() {
    if (AbilitiesToDisplay.IsEmpty()) {
        return;
    }

    auto &[Ability, ABS, ContinueDelegate, bCalled] = *AbilitiesToDisplay.Peek();
    if (ABS.IsStale()) {
        return;
    }
    
    if (auto Spec = ABS->FindAbilitySpecFromHandle(Ability); Spec == nullptr && !Spec->IsActive()) {
        AbilitiesToDisplay.Pop();
        if (AbilitiesToDisplay.IsEmpty()) {
            OnQueueEmpty.Broadcast();
        }
    } else if (!bCalled) {
        check(ContinueDelegate.IsBound())
        ContinueDelegate.Execute();
        bCalled = true;
    }
}

