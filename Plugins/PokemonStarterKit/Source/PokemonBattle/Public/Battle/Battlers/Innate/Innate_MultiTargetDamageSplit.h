// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Innate_MultiTargetDamageSplit.generated.h"

/**
 * Applied a 75% modifier if a move hits multiple targets
 */
UCLASS()
class POKEMONBATTLE_API UInnate_MultiTargetDamageSplit : public UGameplayAbility {
    GENERATED_BODY()

public:
    UInnate_MultiTargetDamageSplit();
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData *TriggerEventData) override;

};
