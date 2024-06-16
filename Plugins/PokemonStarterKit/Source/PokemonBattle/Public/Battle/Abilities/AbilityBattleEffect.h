// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/ManagedTraitHolder.h"
#include "Engine/DataAsset.h"
#include "AbilityBattleEffect.generated.h"

class UDamageModificationTrait;

/**
 * Damage modification traits that apply to an ability
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FAbilityDamageModifiers {
    GENERATED_BODY()

    /**
     * Damage modification traits that apply on moves used by the owner of the ability
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = Damage)
    TArray<TObjectPtr<UDamageModificationTrait>> User;
    
};

/**
 * The effect of an ability in battle
 */
UCLASS()
class POKEMONBATTLE_API UAbilityBattleEffect : public UDataAsset {
    GENERATED_BODY()

public:
    /**
     * Get the damage modifier traits
     * @return Traits that apply to modify the damage value of a move
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    const FAbilityDamageModifiers& GetDamageModifiers() const;

private:
    /**
     * Traits that apply to modify the damage value of a move
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDamageModifiers, Category = Moves)
    FAbilityDamageModifiers DamageModifiers;

};

using FManagedBattleAbility = TManagedTraitHolder<UAbilityBattleEffect, TEXT("/Game/Data/Effects/Abilities/Battle")>;