// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayEffectComponent.h"
#include "GameplayEffectTypes.h"
#include "Battle/Events/TargetedEvents.h"

#include "TypeMatchUpModifierComponent.generated.h"

/**
 * Gameplay effect component for modifying a type match up during damage calculation.
 */
UCLASS(DisplayName = "Modify Type Matchup")
class POKEMONBATTLE_API UTypeMatchUpModifierComponent : public UGameplayEffectComponent {
    GENERATED_BODY()

public:
    bool OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer, FActiveGameplayEffect &ActiveGE) const override;
    
private:
    void OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& GERemovalInfo, UAbilitySystemComponent* ASC, FGameplayTagContainer Container, FDelegateHandle DelegateHandle) const;
    void OnEventReceived(FGameplayTag Tag, const FGameplayEventData* EventData) const;
    
    /**
     * Optional filter of types this should be applied to. (If left empty apply to all types so long as the match up
     * conditions apply).
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TSet<FName> AttackingTypes;

    /**
     * Optional filter of types this should be applied to. (If left empty apply to all types so long as the match up
     * conditions apply).
     */
    UPROPERTY(EditAnywhere, meta = (GetOptions = "PokemonData.TypeHelper.GetTypeNames"))
    TSet<FName> DefendingTypes;

    /**
     * The scope(s) that must be accounted for. Must contain at least one element or this will never trigger.
     */
    UPROPERTY(EditAnywhere)
    TArray<ETargetedEventScope> Scope;
    
    /**
     * What effectiveness(es) should this be applied to?
     */
    UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = EDamageEffectiveness))
    uint8 ApplyFor;
    
    /**
     * The numeric operation of this modifier: Override, Add, Multiply, etc
     */
    UPROPERTY(EditAnywhere)
    TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

    /**
     * Magnitude of the modifier
     */
    UPROPERTY(EditAnywhere)
    float Modifier;

};
