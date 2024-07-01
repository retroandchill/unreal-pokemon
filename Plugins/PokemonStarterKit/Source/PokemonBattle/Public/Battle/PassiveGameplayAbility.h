// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PassiveGameplayAbility.generated.h"

/**
 * Gameplay ability that activates immediately upon the battler being created.
 */
UCLASS()
class POKEMONBATTLE_API UPassiveGameplayAbility : public UGameplayAbility {
    GENERATED_BODY()

public:
    void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) override;

};
