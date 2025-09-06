// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "Innate_DamageSwing.generated.h"

/**
 * Apply a [85%, 100%] damage swing to the move's damage
 */
UCLASS()
class POKEMONBATTLE_API UInnate_DamageSwing : public UGameplayAbility
{
    GENERATED_BODY()

  public:
    UInnate_DamageSwing();
    void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo *ActorInfo,
                         const FGameplayAbilityActivationInfo ActivationInfo,
                         const FGameplayEventData *TriggerEventData) override;
};
