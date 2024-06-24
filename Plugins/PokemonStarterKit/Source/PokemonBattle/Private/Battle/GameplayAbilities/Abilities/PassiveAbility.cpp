// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Abilities/PassiveAbility.h"
#include "AbilitySystemComponent.h"

void UPassiveAbility::OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) {
    Super::OnAvatarSet(ActorInfo, Spec);
    ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}