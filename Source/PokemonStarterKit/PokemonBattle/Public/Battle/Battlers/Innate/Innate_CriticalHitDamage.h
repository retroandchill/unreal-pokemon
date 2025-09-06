// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Innate_CriticalHitDamage.generated.h"

/**
 * The damage applied when a critical hit is scored.
 */
UCLASS()
class POKEMONBATTLE_API UInnate_CriticalHitDamage : public UGameplayAbility
{
    GENERATED_BODY()

  public:
    UInnate_CriticalHitDamage();
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                         const FGameplayAbilityActivationInfo ActivationInfo,
                         const FGameplayEventData *TriggerEventData) override;
};
