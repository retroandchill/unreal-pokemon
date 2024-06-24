// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PassiveAbility.generated.h"

/**
 * Gameplay ability that grants a passive effect to the actor.
 */
UCLASS()
class POKEMONBATTLE_API UPassiveAbility : public UGameplayAbility {
    GENERATED_BODY()

public:
    void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) override;

};
