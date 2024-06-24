// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PassiveAbility.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_SameTypeAttackBonus.generated.h"

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UGameplayAbility_SameTypeAttackBonus : public UPassiveAbility {
    GENERATED_BODY()

public:
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;

};
