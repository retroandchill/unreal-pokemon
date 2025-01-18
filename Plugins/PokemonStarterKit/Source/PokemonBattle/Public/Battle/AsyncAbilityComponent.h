// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "UE5Coro.h"
#include "AsyncAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POKEMONBATTLE_API UAsyncAbilityComponent : public UAbilitySystemComponent {
    GENERATED_BODY()

public:
    UE5Coro::TCoroutine<int32> HandleGameplayEventAsync(FGameplayTag EventTag, const FGameplayEventData *Payload, FForceLatentCoroutine = {});

    UE5Coro::TCoroutine<bool> TriggerAbilityFromGameplayEventAsync(FGameplayAbilitySpecHandle AbilityToTrigger, FGameplayAbilityActorInfo* ActorInfo, FGameplayTag Tag, const FGameplayEventData* Payload, UAbilitySystemComponent& Component);

};