// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/Damage/DamageModificationTraits.h"
#include "Battle/Traits/IndividualTraitHolder.h"
#include "Battle/Traits/ManagedTraitHolder.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTraits.h"
#include "Engine/DataAsset.h"

#include "AbilityBattleEffect.generated.h"

class UGameplayEffect;
/**
 * The effect of an ability in battle
 */
UCLASS()
class POKEMONBATTLE_API UAbilityBattleEffect : public UDataAsset, public IIndividualTraitHolder {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    const FIndividualDamageModifierTraits &GetDamageModifiers() const final;
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    const FIndividualCriticalHitRateModifierTraits &GetCriticalHitRateModifiers() const final;

    bool HasTag(FName Tag) const final;

  private:
    /**
     * Traits that apply to modify the damage value of a move
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDamageModifiers, Category = Moves)
    FIndividualDamageModifierTraits DamageModifiers;

    /**
     * Traits that apply to modify the critical hit rate of a move
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetCriticalHitRateModifiers, Category = Moves)
    FIndividualCriticalHitRateModifierTraits CriticalHitRateModifiers;

    /**
     * The set of metadata tags associated with the ability
     */
    UPROPERTY(EditAnywhere, Category = Metadata)
    TSet<FName> Tags;
};

using FManagedBattleAbility = TManagedTraitHolder<UAbilityBattleEffect, TEXT("/Game/Data/Effects/Abilities/Battle")>;