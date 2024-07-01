// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/PassiveGameplayAbility.h"
#include "AbilitySystemComponent.h"

void UPassiveGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);
    ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}