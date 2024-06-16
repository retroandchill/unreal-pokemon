// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Traits/IndividualTraitHolder.h"
#include "Battle/Traits/ManagedTraitHolder.h"
#include "Battle/Traits/Damage/DamageModificationTraits.h"
#include "Engine/DataAsset.h"
#include "AbilityBattleEffect.generated.h"

/**
 * The effect of an ability in battle
 */
UCLASS()
class POKEMONBATTLE_API UAbilityBattleEffect : public UDataAsset, public IIndividualTraitHolder {
    GENERATED_BODY()

public:
    /**
     * Get the damage modifier traits
     * @return Traits that apply to modify the damage value of a move
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Moves)
    const FIndividualDamageModifierTraits& GetDamageModifiers() const final;

private:
    /**
     * Traits that apply to modify the damage value of a move
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetDamageModifiers, Category = Moves)
    FIndividualDamageModifierTraits DamageModifiers;

};

using FManagedBattleAbility = TManagedTraitHolder<UAbilityBattleEffect, TEXT("/Game/Data/Effects/Abilities/Battle")>;