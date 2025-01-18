// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/AsyncAbilityComponent.h"
#include "RetroLib/Functional/Delegates.h"


UE5Coro::TCoroutine<int32> UAsyncAbilityComponent::HandleGameplayEventAsync(FGameplayTag EventTag,
                                                                            const FGameplayEventData *Payload,
                                                                            FForceLatentCoroutine) {
    int32 TriggeredCount = 0;
    auto CurrentTag = EventTag;
    FScopedAbilityListLock ActiveScopeLock(*this);;
    while (CurrentTag.IsValid()) {
        if (GameplayEventTriggeredAbilities.Contains(CurrentTag)) {
            for (auto TriggeredAbilityHandles = GameplayEventTriggeredAbilities[CurrentTag]; auto &AbilityHandle : TriggeredAbilityHandles) {
                if (co_await TriggerAbilityFromGameplayEventAsync(AbilityHandle, AbilityActorInfo.Get(), EventTag, Payload, *this)) {
                    TriggeredCount++;
                }
            }
        }

        CurrentTag = CurrentTag.RequestDirectParent();
    }

    if (FGameplayEventMulticastDelegate *Delegate = GenericGameplayEventCallbacks.Find(EventTag)) {
        // Make a copy before broadcasting to prevent memory stomping
        FGameplayEventMulticastDelegate DelegateCopy = *Delegate;
        DelegateCopy.Broadcast(Payload);
    }

    // Make a copy in case it changes due to callbacks
    TArray<TPair<FGameplayTagContainer, FGameplayEventTagMulticastDelegate>> LocalGameplayEventTagContainerDelegates =
        GameplayEventTagContainerDelegates;
    for (TPair<FGameplayTagContainer, FGameplayEventTagMulticastDelegate> &SearchPair :
         LocalGameplayEventTagContainerDelegates) {
        if (SearchPair.Key.IsEmpty() || EventTag.MatchesAny(SearchPair.Key)) {
            SearchPair.Value.Broadcast(EventTag, Payload);
        }
    }

    co_return TriggeredCount;
}

UE5Coro::TCoroutine<bool> UAsyncAbilityComponent::TriggerAbilityFromGameplayEventAsync(
    FGameplayAbilitySpecHandle AbilityToTrigger, FGameplayAbilityActorInfo *ActorInfo, FGameplayTag Tag,
    const FGameplayEventData *Payload, UAbilitySystemComponent &Component) {
    auto FutureState = MakeShared<TFutureState<int32>>();
    Retro::Delegates::TScopedBinding Binding(OnAbilityEnded, [&FutureState, AbilityToTrigger](const FAbilityEndedData& Data) {
        if (Data.AbilitySpecHandle == AbilityToTrigger) {
            FutureState->EmplaceResult(0);
        }
    });
    if (!TriggerAbilityFromGameplayEvent(AbilityToTrigger, ActorInfo, Tag, Payload, Component)) {
        co_return false;
    }

    co_await TFuture<void>(FutureState);
    co_return true;
}