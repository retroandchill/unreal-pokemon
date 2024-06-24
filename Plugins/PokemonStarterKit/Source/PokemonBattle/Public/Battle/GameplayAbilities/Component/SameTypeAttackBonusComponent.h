// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffectComponent.h"
#include "GameplayTagContainer.h"

#include "SameTypeAttackBonusComponent.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectRemovalInfo;

/**
 * Apply a bonus to any move of the same type as the user.
 */
UCLASS(DisplayName = "Apply Same-Type Attack Bonus")
class POKEMONBATTLE_API USameTypeAttackBonusComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

public:
    bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer, FActiveGameplayEffect &ActiveGE) const override;
    
private:
    /**
     * We need to be notified when we're removed to unregister some callbacks
     * 
     */
    void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& GERemovalInfo, UAbilitySystemComponent* ASC, TArray<TTuple<FGameplayTag, FDelegateHandle>> AllBoundEvents) const;

    /**
     * We need to register a callback for when the owner changes its tags.  When that happens, we need to figure out if
     * our GE should continue to execute
     * 
     */
    void OnTagChanged(const FGameplayTag GameplayTag, int32 NewCount, FActiveGameplayEffectHandle ActiveGEHandle) const;
};
