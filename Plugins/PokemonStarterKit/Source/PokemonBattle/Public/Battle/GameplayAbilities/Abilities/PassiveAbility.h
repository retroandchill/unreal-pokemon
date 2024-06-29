// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PassiveAbility.generated.h"

/**
 * Basic passive ability definition, for an ability that is activated the second it is applied.
 */
UCLASS()
class POKEMONBATTLE_API UPassiveAbility : public UGameplayAbility {
    GENERATED_BODY()

public:
    void OnAvatarSet(const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilitySpec &Spec) override;

};
